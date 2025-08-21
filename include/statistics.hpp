#pragma once

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"
#include <algorithm>
#include <cstddef>
#include <flat_map>
#include <iterator>
#include <numeric>
#include <print>
#include <random>
#include <unordered_map>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    const auto &books = cont.GetBooks();

    std::flat_map<std::string_view, size_t, Comparator> temp_map(comp);
    for (const auto &book : books) {
        std::string_view author_view = book._author;
        auto it = temp_map.find(author_view);
        if (it != temp_map.end()) {
            it->second++;
        } else {
            temp_map.emplace(author_view, 1);
        }
    }

    return temp_map;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &db) {
    if (db.size() == 0)
        return 0.0;
    return std::transform_reduce(db.cbegin(), db.cend(), 0.0, std::plus{},
                                 [](const Book &b) -> double { return b._rating; }) /
           db.size();
}

std::flat_map<Genre, double> calculateGenreRatings(BookIterator auto first, BookIterator auto last) {

    std::unordered_map<Genre, std::pair<double, size_t>> buf;

    buf.reserve(static_cast<size_t>(Genre::Unknown));

    std::for_each(first, last, [&buf](const Book &book) {
        auto &[sum, cnt] = buf[book._genre];
        sum += book._rating;
        cnt++;
    });

    std::flat_map<Genre, double> result;
    std::transform(buf.begin(), buf.end(), std::inserter(result, result.end()), [](auto &item) {
        auto &[sum, cnt] = item.second;
        return std::make_pair(item.first, sum / cnt);
    });

    return result;
}

template <BookContainerLike Container, BookComparator Comparator>
std::vector<std::reference_wrapper<const Book>> getTopNBy(BookDatabase<Container> &db, size_t N, Comparator &&comp) {
    N = N < db.size() ? N : db.size();
    std::vector<std::reference_wrapper<const Book>> out;
    out.reserve(N);

    std::partial_sort(db.begin(), db.begin() + N, db.end(),
                      [&](const auto &a, const auto &b) { return !std::invoke(comp, a, b); });

    out.insert(out.end(), db.begin(), db.begin() + N);

    return out;
}

template <BookContainerLike Container>
std::vector<std::reference_wrapper<const Book>> sampleRandomBooks(BookDatabase<Container> &db, size_t N) {
    N = N < db.size() ? N : db.size();
    std::vector<std::reference_wrapper<const Book>> out;
    out.reserve(N);

    std::sample(db.cbegin(), db.cend(), std::back_inserter(out), N, std::mt19937{std::random_device{}()});

    return out;
}

}  // namespace bookdb
