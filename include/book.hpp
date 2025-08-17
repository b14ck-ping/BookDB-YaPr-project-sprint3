#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(const std::string_view &s) {
    using bookdb::Genre;
    if (s == std::string_view("Fiction"))
        return Genre::Fiction;
    else if (s == std::string_view("Mistery"))
        return Genre::Fiction;
    else if (s == std::string_view("NonFiction"))
        return Genre::Fiction;
    else if (s == std::string_view("SciFi"))
        return Genre::Fiction;
    else if (s == std::string_view("Biography"))
        return Genre::Fiction;
    else
        return Genre::Unknown;
}

constexpr std::string GenreToString(Genre g) {
    using bookdb::Genre;
    switch (g) {
    case Genre::Fiction:
        return "Fiction";
    case Genre::Mystery:
        return "Mystery";
    case Genre::NonFiction:
        return "NonFiction";
    case Genre::SciFi:
        return "SciFi";
    case Genre::Biography:
        return "Biography";
    default:
        return "Unknown";
    }
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view _author;
    std::string _title;

    int _year;
    Genre _genre;
    double _rating;
    int _read_count;

    // Ваш код для конструкторов здесь
    constexpr Book(std::string title, std::string_view author, int year, std::string genre, double rating,
                   int read_count)
        : _author(author), _title(title), _year(year), _genre(GenreFromString(genre)), _rating(rating),
          _read_count(read_count) {}

    constexpr Book(std::string title, std::string_view author, int year, Genre genre, double rating, int read_count)
        : _author(author), _title(title), _year(year), _genre(genre), _rating(rating), _read_count(read_count) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Ваш код для std::formatter<Book> здесь
template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &b, FormatContext &fc) const {
        return format_to(fc.out(), "Title:\"{}\", Author: \"{}\", Year: {}, Genre: {}, Rating: {}, Read count: {}",
                         b._title, b._author, b._year, b._genre, b._rating, b._read_count);
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std
