#pragma once

#include "book.hpp"
#include "concepts.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace bookdb {

inline auto YearBetween(int YearFrom, int YearTo) noexcept {
    return [YearFrom, YearTo](Book &book) { return book._year >= YearFrom && book._year <= YearTo; };
}

inline auto RatingAbove(double rating) noexcept {
    return [rating](Book &book) { return book._rating >= rating; };
}

inline auto GenreIs(Genre genre) noexcept {
    return [genre](Book &book) { return book._genre == genre; };
}

inline auto AuthorIs(std::string author) noexcept {
    return [author](Book &book) { return book._author == author; };
}

template <BookPredicate... Predicates>
auto all_of(Predicates... preds) noexcept {
    return [... preds = std::move(preds)](Book &book) { return (preds(book) && ...); };
}

template <BookPredicate... Predicates>
auto any_of(Predicates... preds) noexcept {
    return [... preds = std::move(preds)](Book &book) -> bool { return (preds(book) || ...); };
}

template <BookIterator Iterator, BookPredicate Predicate>
static std::vector<std::reference_wrapper<const Book>> filterBooks(Iterator first, Iterator last,
                                                                   Predicate &&pred) noexcept {
    std::vector<std::reference_wrapper<const Book>> out;
    std::copy_if(first, last, std::back_inserter(out),
                 [pred = std::forward<Predicate>(pred)](Book &book) { return pred(book); });

    return out;
}

}  // namespace bookdb