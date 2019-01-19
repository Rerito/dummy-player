
#pragma once

#include <functional>
#include <vector>
#include <string>

#include "type_conversion.hpp"
#include "meta/type_list.hpp"
#include "meta/nth_element.hpp"


namespace detail {

template <typename FTor, typename Ret, typename... Args, size_t... Is>
auto apply(FTor f, std::vector<std::string> const& args, meta::type_list<Ret, Args...>, std::index_sequence<Is...>) 
    -> std::enable_if_t<!std::is_void<Ret>::value, std::string> {
    using std::to_string;
    return to_string(f(from_string<nth_element<Is, Args...> >{}(args.at(Is))...));
}

template <typename FTor, typename Ret, typename... Args, size_t... Is>
auto apply(FTor f, std::vector<std::string> const& args, meta::type_list<Ret, Args...>, std::index_sequence<Is...>) 
    -> std::enable_if_t<std::is_void<Ret>::value, std::string> {
    f(from_string<nth_element<Is, Args...> >{}(args.at(Is))...);
    return {};
}

} // namespace detail

template <size_t N, typename FTor, typename ArgsList>
auto apply(FTor f, std::vector<std::string> const& args, ArgsList arg_list) {
    return detail::apply(f, args, arg_list, std::make_index_sequence<N>{});
}

template <typename Sig, Sig *Fn>
class command;

// This abstraction layer allows us to offer the same interface facade
// For every user command (text-driven).
// Then, using some TMP we are able to dispatch arguments accordingly...
template <typename Ret, typename... Args, Ret(*Fn)(Args...)>
class command<Ret(Args...), Fn> {
    
public:
    auto operator()(std::vector<std::string> const& args) const {
        return apply<sizeof... (Args)>(Fn, args, meta::type_list<Ret, Args...>{});
    }
};

