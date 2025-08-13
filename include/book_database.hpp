#pragma once

#include <initializer_list>
#include <iterator>
#include <print>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using iterator = BookContainer::iterator;
    static_assert(BookIterator<iterator>, "Iterator doesn't match to concept");

    using const_iterator = BookContainer::iterator;
    static_assert(BookIterator<iterator>, "Const iterator doesn't match to concept");

    using AuthorContainer = std::set<std::string>;
    // Ваш код здесь

    BookDatabase() = default;

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    template <typename... Args>
    void EmplaceBack(Args... args) {
        auto tuple_args = std::forward_as_tuple(std::forward<Args>(args)...);
        const std::string &autor_str = std::get<1>(tuple_args);

        auto [it, _] = authors_.emplace(autor_str);

        auto &&title = std::get<0>(tuple_args);
        std::string_view author_view = *it;
        auto &&year = std::get<2>(tuple_args);
        auto &&genre = std::get<3>(tuple_args);
        auto &&rating = std::get<4>(tuple_args);
        auto &&read_count = std::get<5>(tuple_args);

        books_.emplace_back(title, author_view, year, genre, rating, read_count);
    }

    auto begin() { return books_.begin(); }

    auto end() { return books_.end(); }

    size_t size() const { return books_.size(); }

    auto GetBooks() { return books_; }
    auto GetBooks() const { return books_; }

    auto GetAuthors() { return authors_; }
    auto GetAuthors() const { return authors_; }

    // Ваш код здесь

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
