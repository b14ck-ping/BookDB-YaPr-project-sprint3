#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_trasparent = void;
};

struct TransparentStringEqual {};

struct TransparentStringHash {};

}  // namespace bookdb
