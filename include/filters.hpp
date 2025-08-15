#pragma once

#include <algorithm>
#include <functional>
#include <utility>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "concepts.hpp"

namespace bookdb {

inline auto YearBetween(int YearFrom, int YearTo) noexcept {
    return [YearFrom, YearTo](Book &book) { return book._year >= YearFrom && book._year <= YearTo; };
}

inline auto RatingAbove(double rating) {
    return [rating](Book &book) { return book._rating >= rating; };
}

inline auto GenreIs(Genre genre) {
    return [genre](Book &book) { return book._genre == genre; };
}

template <BookPredicate... Predicates>
auto all_of(Predicates... preds) {
    return [... preds = std::move(preds)](Book &book) { return (preds(book) && ...); };
}

template <BookPredicate... Predicates>
auto any_of(Predicates... preds) {
    return [... preds = std::move(preds)](Book &book) -> bool { return (preds(book) || ...); };
}

template <BookIterator Iterator, BookPredicate predicate>
std::vector<std::reference_wrapper<Book>> filterBooks(Iterator first, Iterator last, predicate &&pred) {
    std::vector<std::reference_wrapper<Book>> out;
    std::for_each(first, last, [&out, &pred](Book &book) {
        if (std::forward<predicate>(pred)(book)) {
            out.push_back(std::reference_wrapper<Book>(book));
        }
    });

    return out;
}

// template <BookContainerLike Container, BookPredicate predicate>
// std::vector<std::reference_wrapper<const Book>> sampleRandomBooks() {}

template <BookContainerLike Container, BookComparator Comparator>
std::vector<std::reference_wrapper<const Book>> getTopNBy(BookDatabase<Container> &db, size_t N, Comparator &&comp) {
    std::vector<std::reference_wrapper<const Book>> out;

    std::partial_sort(db.begin(), db.end(), std::forward<Comparator>(comp));

    std::for_each_n(db.rend(), db.rbegin(), N,
                    [&out](const Book &book) { out.push_back(std::reference_wrapper<const Book>(book)); });
    return out;
}

}  // namespace bookdb