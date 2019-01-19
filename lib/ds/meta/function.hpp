
#pragma once

namespace meta {

namespace detail {
template <typename Sig>
struct fun_ptr;

template <typename Ret, typename... Args>
struct fun_ptr<Ret(Args...)> {
    using type = Ret(*)(Args...);
};

} // namespace detail

// I don't like the function pointer syntax much so let's just add some
// syntactic sugar over it...
template <typename Sig>
using function_ptr = typename detail::fun_ptr<Sig>::type;

} // namespace meta
