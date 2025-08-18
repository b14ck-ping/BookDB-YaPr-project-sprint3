#pragma once

#include "book.hpp"
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(const std::string &a, const std::string &b) const { return a < b; }

    bool operator()(const std::string &a, const std::string_view b) const { return a < b; }

    bool operator()(const std::string_view a, const std::string &b) const { return a < b; }

    bool operator()(const std::string_view a, const std::string_view &b) const { return a < b; }

    bool operator()(const std::string_view a, const char *b) const { return a < b; }

    bool operator()(const std::string a, const char *b) const { return a < b; }

    bool operator()(const char *a, const std::string_view &b) const { return a < b; }

    bool operator()(const char *a, const std::string &b) const { return a < b; }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(const std::string &a, const std::string &b) const { return a == b; }

    bool operator()(const std::string &a, std::string_view b) const { return a == b; }

    bool operator()(std::string_view a, const std::string &b) const { return a == b; }

    bool operator()(std::string_view a, const std::string_view &b) const { return a == b; }

    bool operator()(std::string_view a, const char *b) const { return a == b; }

    bool operator()(std::string a, const char *b) const { return a == b; }

    bool operator()(const char *a, std::string_view &b) const { return a == b; }

    bool operator()(const char *a, std::string &b) const { return a == b; }
};

struct TransparentStringHash {
    using is_transparent = void;

    size_t operator()(const char *str) const { return std::hash<std::string_view>{}(str); }

    size_t operator()(std::string_view str) const { return std::hash<std::string_view>{}(str); }

    size_t operator()(const std::string &str) const { return std::hash<std::string>{}(str); }
};

}  // namespace bookdb
