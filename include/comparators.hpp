#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs._author < rhs._author; }
};

struct LessByYear {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs._year < rhs._year; }
};

struct LessByTitle {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs._title < rhs._title; }
};

struct LessByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs._rating < rhs._rating; }
};

struct LessByReadCount {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const {
        return lhs._read_count < rhs._read_count;
    }
};

struct LessByGenre {
    bool operator()(const bookdb::Book &lhs, bookdb::Book &rhs) const { return lhs._genre < rhs._genre; }
};

struct GreaterThanRating {
    bool operator()(const bookdb::Book &b, double rr) const { return b._rating > rr; }
};

struct MatchGenre {
    bool operator()(const bookdb::Book &b, const bookdb::Genre genre) const { return b._genre == genre; }
};

}  // namespace bookdb::comp