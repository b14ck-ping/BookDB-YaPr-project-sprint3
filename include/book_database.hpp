#pragma once

#include "book.hpp"
#include "concepts.hpp"
#include <initializer_list>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

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

    using value_type = BookContainer::value_type;
    using reference = BookContainer::reference;
    using const_reference = BookContainer::const_reference;

    using AuthorContainer = std::unordered_set<std::string>;
    // Ваш код здесь

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) {
        for (const auto &book : books) {
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
        auto &newBook = books_.emplace_back(std::forward<Args>(args)...);
        auto [it, _] = authors_.emplace(newBook._author);
        newBook._author = *it;
        return newBook;
    }

    void push_back(Book book) {
        auto [it, _] = authors_.emplace(book._author);
        book._author = *it;
        books_.push_back(book);
    }

    void push_back(Book &&book) {
        auto [it, _] = authors_.emplace(book._author);
        book._author = *it;
        books_.push_back(std::move(book));
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
