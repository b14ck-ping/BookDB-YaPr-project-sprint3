#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const bookdb::Book &lhr, const bookdb::Book &rhr) const { return lhr._author < rhr._author; }
};

struct LessByYear {
    bool operator()(const bookdb::Book &lhr, const bookdb::Book &rhr) const { return lhr._year < rhr._year; }
};

struct LessByTitle {
    bool operator()(const bookdb::Book &lhr, const bookdb::Book &rhr) const { return lhr._title < rhr._title; }
};

struct LessByRating {
    bool operator()(const bookdb::Book &lhr, const bookdb::Book &rhr) const { return lhr._rating < rhr._rating; }
};

struct LessByReadCount {
    bool operator()(const bookdb::Book &lhr, const bookdb::Book &rhr) const {
        return lhr._read_count < rhr._read_count;
    }
};

struct LessByReadGenre {
    bool operator()(const bookdb::Book &lhr, bookdb::Book &rhr) const { return lhr._genre < rhr._genre; }
};

struct GreaterThanRating {
    bool operator()(const bookdb::Book &b, double rr) const { return b._rating > rr; }
};

struct MatchGenre {
    bool operator()(const bookdb::Book &b, const bookdb::Genre genre) const { return b._genre == genre; }
};

}  // namespace bookdb::comp