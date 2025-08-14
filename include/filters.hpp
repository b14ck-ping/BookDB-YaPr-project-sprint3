#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"

namespace bookdb {

/* YearBetween

    RatingAbove

        GenreIs

            all_of

                any_of */

// auto RatingAbove(const bookdb::Book &b, double rr) {};

// struct MatchGenre {
//     bool operator()(const bookdb::Book &b, const bookdb::Genre genre) const { return b._genre == genre; }
// };

}  // namespace bookdb