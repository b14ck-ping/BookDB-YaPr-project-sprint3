#include "book.hpp"
#include "filters.hpp"
#include <gtest/gtest.h>

using namespace bookdb;

static Book book1 = {"The Lord of the Rings", "Tolkien", 1954, "Fiction", 4.9, 1000};
static Book book2 = {"Dune", "Herbert", 1965, "SciFi", 4.7, 1500};
static Book book3 = {"1984", "Orwell", 1949, "Fiction", 4.8, 2000};
static Book book4 = {"Foundation", "Asimov", 1951, "SciFi", 4.6, 1800};

std::vector<Book> books = {book1, book2, book3, book4};

TEST(FilterTest, YearBetweenFilter) {
    auto filter = YearBetween(1950, 1960);

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
    EXPECT_FALSE(filter(book3));
    EXPECT_TRUE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get()._title, "The Lord of the Rings");
    EXPECT_EQ(result[1].get()._title, "Foundation");
}

TEST(FilterTest, RatingAboveFilter) {
    auto filter = RatingAbove(4.75);

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
    EXPECT_TRUE(filter(book3));
    EXPECT_FALSE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get()._title, "The Lord of the Rings");
    EXPECT_EQ(result[1].get()._title, "1984");
}

TEST(FilterTest, GenreIsFilter) {
    auto filter = GenreIs(Genre::Fiction);

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
    EXPECT_TRUE(filter(book3));
    EXPECT_FALSE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get()._title, "The Lord of the Rings");
    EXPECT_EQ(result[1].get()._title, "1984");
}

TEST(FilterTest, AuthorIsFilter) {
    auto filter = AuthorIs("Tolkien");

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
    EXPECT_FALSE(filter(book3));
    EXPECT_FALSE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].get()._title, "The Lord of the Rings");
}

TEST(FilterTest, AllOfFilter) {
    auto filter = all_of(YearBetween(1950, 1960), RatingAbove(4.5), GenreIs(Genre::Fiction));

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
    EXPECT_FALSE(filter(book3));
    EXPECT_FALSE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].get()._title, "The Lord of the Rings");
}

TEST(FilterTest, AnyOfFilter) {
    auto filter = any_of(AuthorIs("Herbert"), AuthorIs("Asimov"));

    EXPECT_FALSE(filter(book1));
    EXPECT_TRUE(filter(book2));
    EXPECT_FALSE(filter(book3));
    EXPECT_TRUE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get()._title, "Dune");
    EXPECT_EQ(result[1].get()._title, "Foundation");
}

TEST(FilterTest, EmptyResults) {
    auto strict_filter = all_of(YearBetween(2000, 2020), RatingAbove(5.0), GenreIs(Genre::Mystery));

    auto result = filterBooks(books.begin(), books.end(), strict_filter);
    EXPECT_TRUE(result.empty());

    auto author_filter = AuthorIs("Stephen King");
    result = filterBooks(books.begin(), books.end(), author_filter);
    EXPECT_TRUE(result.empty());
}

TEST(FilterTest, EdgeCases) {
    Book edge_book = {"Edge", "Author", 2000, "Fiction", 0.0, 0};

    auto year_filter = YearBetween(2000, 2000);
    EXPECT_TRUE(year_filter(edge_book));

    auto rating_filter = RatingAbove(0.0);
    EXPECT_TRUE(rating_filter(edge_book));

    auto genre_filter = GenreIs(Genre::Unknown);
    EXPECT_FALSE(genre_filter(edge_book));

    auto author_filter = AuthorIs("");
    EXPECT_FALSE(author_filter(edge_book));
}

TEST(FilterTest, CombinedFilters) {
    auto filter = all_of(YearBetween(1940, 1960), any_of(GenreIs(Genre::SciFi), AuthorIs("Tolkien")));

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
    EXPECT_FALSE(filter(book3));
    EXPECT_TRUE(filter(book4));

    auto result = filterBooks(books.begin(), books.end(), filter);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get()._title, "The Lord of the Rings");
    EXPECT_EQ(result[1].get()._title, "Foundation");
}
