#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"

#include <print>

namespace bookdb {

// template <BookContainerLike T, typename Comparator = TransparentStringLess>
// auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {}

// template <BookContainerLike T, typename Comparator>
// auto buildAuthorHistogram(const BookDatabase<T> &cont, Comparator comp = {}) {}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &db) {
    return std::accumulate(db.cbegin(), db.cend(), 0.0, [](double acc, const Book &b) { return acc += b._rating; }) /
           db.size();
}

template <BookIterator Iterator>
std::flat_map<Genre, double> calculateGenreRatings(Iterator first, Iterator last) {

    std::unordered_map<Genre, std::pair<double, size_t>> buf;

    buf.reserve(static_cast<size_t>(Genre::Unknown));

    std::for_each(first, last, [&buf](const Book &book) {
        auto &[sum, cnt] = buf[book._genre];
        sum += book._rating;
        cnt++;
    });

    std::flat_map<Genre, double> result;
    std::for_each(buf.begin(), buf.end(), [&result](auto &item) {
        auto &[sum, cnt] = item.second;
        result.emplace(item.first, sum / cnt);
    });

    return result;
}

}  // namespace bookdb

namespace std {
template <>
struct formatter<std::flat_map<bookdb::Genre, double>> {
    template <typename FormatContext>
    auto format(const std::flat_map<bookdb::Genre, double> &in, FormatContext &fc) const {
        for (const auto &[genre, avRating] : in)
            format_to(fc.out(), "Genre: {}, Average rating: {}\n", genre, avRating);
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std