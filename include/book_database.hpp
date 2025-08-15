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

    using const_iterator = BookContainer::const_iterator;
    static_assert(BookIterator<const_iterator>, "Const iterator doesn't match to concept");

    using reverse_iterator = BookContainer::reverse_iterator;
    static_assert(BookIterator<reverse_iterator>, "Reverse iterator doesn't match to concept");

    using AuthorContainer = std::set<std::string>;
    // Ваш код здесь

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) {
        for (auto book : books) {
            this->push_back(book);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    template <typename... Args>
    Book &EmplaceBack(Args... args) {
        auto tuple_args = std::forward_as_tuple(std::forward<Args>(args)...);
        const std::string &autor_str = std::get<1>(tuple_args);

        auto [it, _] = authors_.emplace(autor_str);

        auto &&title = std::get<0>(tuple_args);
        std::string_view author_view = *it;
        auto &&year = std::get<2>(tuple_args);
        auto &&genre = std::get<3>(tuple_args);
        auto &&rating = std::get<4>(tuple_args);
        auto &&read_count = std::get<5>(tuple_args);

        return books_.emplace_back(title, author_view, year, genre, rating, read_count);
    }

    void push_back(Book &book) {
        auto [it, _] = authors_.emplace(book._author);
        book._author = *it;
        books_.push_back(book);
    }

    void push_back(Book &&book) {
        auto [it, _] = authors_.emplace(book._author);
        book._author = *it;
        books_.push_back(book);
    }

    iterator begin() { return books_.begin(); }
    const_iterator cbegin() const { return books_.cbegin(); }
    reverse_iterator rbegin() { return books_.rbegin(); }

    iterator end() { return books_.end(); }
    const_iterator cend() const { return books_.cend(); }
    reverse_iterator rend() { return books_.rend(); }

    size_t size() const { return std::distance(books_.begin(), books_.end()); }

    BookContainer &GetBooks() { return books_; }
    const BookContainer &GetBooks() const { return books_; }

    AuthorContainer &GetAuthors() { return authors_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

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
