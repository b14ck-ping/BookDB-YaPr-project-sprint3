#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T &t) {
    { t.emplace_back(std::declval<typename T::value_type>()) };
    { t.push_back(std::declval<typename T::value_type>()) };
    { t.size() } -> std::same_as<size_t>;
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    { t.clear() };
};

template <typename T>
concept BookIterator = std::random_access_iterator<T> || std::contiguous_iterator<T>;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = std::predicate<P, Book &>;

template <typename C>
concept BookComparator = std::invocable<C, const Book &, const Book &> &&
                         std::convertible_to<std::invoke_result_t<C, const Book &, const Book &>, bool>;
}  // namespace bookdb