#pragma once

#include <utility>

namespace detail {

template <size_t, typename>
struct indexed_ {};

template <typename ISeq, typename... Args>
struct indexer_;

template <size_t... Is, typename... Args>
struct indexer_<std::index_sequence<Is...>, Args...> : indexed_<Is, Args>... {};	

template <size_t N, typename T>
T select(indexed_<N, T>);

} // namespace detail

template <size_t N, typename... Args>
using nth_element = decltype(detail::select<N>(detail::indexer_<std::make_index_sequence<sizeof...(Args)>, Args...> {}));