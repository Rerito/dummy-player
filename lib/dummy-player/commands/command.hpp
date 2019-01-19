
#pragma once

#include <functional>
#include <vector>
#include <string>

#include "type_conversion.hpp"
#include "meta/type_list.hpp"
#include "meta/nth_element.hpp"


namespace detail {

template <typename FTor, typename... Args, size_t... Is>
auto apply(FTor f, std::vector<std::string> const& args, meta::type_list<Args...>, std::index_sequence<Is...>) {
    return f(type_conversion<nth_element<Is, Args...> >::from_string(args.at(Is))...);
}

} // namespace detail

template <size_t N, typename FTor, typename ArgsList>
auto apply(FTor f, std::vector<std::string> const& args, ArgsList arg_list) {
    return detail::apply(f, args, arg_list, std::make_index_sequence<N>{});
}

template <typename Sig, Sig *Fn>
class command;

template <typename Ret, typename... Args, Ret(*Fn)(Args...)>
class command<Ret(Args...), Fn> {
    
public:
    auto operator()(std::vector<std::string> const& args) const {
        return apply<sizeof... (Args)>(Fn, args, meta::type_list<Args...>{});
    }
};

