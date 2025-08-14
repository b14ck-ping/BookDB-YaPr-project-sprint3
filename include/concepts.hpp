#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>

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
concept BookIterator = requires(T &t) { requires std::random_access_iterator<T>; };

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = true;

template <typename C>
concept BookComparator = true;

}  // namespace bookdb