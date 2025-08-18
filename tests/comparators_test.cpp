#include "book.hpp"
#include "comparators.hpp"
#include <gtest/gtest.h>

using namespace bookdb;
using namespace bookdb::comp;

static Book book1 = {"The Lord of the Rings", "Tolkien", 1954, "Fiction", 4.9, 1000};
static Book book2 = {"Dune", "Herbert", 1965, "SciFi", 4.7, 1500};
static Book book3 = {"1984", "Orwell", 1949, "Fiction", 4.8, 2000};
static Book book4 = {"Dune", "Herbert", 1965, "SciFi", 4.7, 1500};

TEST(ComparatorTest, LessByAuthor) {
    LessByAuthor comp;

    EXPECT_TRUE(comp(book2, book1));
    EXPECT_FALSE(comp(book1, book2));

    EXPECT_TRUE(comp(book3, book1));
    EXPECT_FALSE(comp(book1, book3));

    EXPECT_TRUE(comp(book2, book3));
    EXPECT_FALSE(comp(book3, book2));

    EXPECT_FALSE(comp(book2, book4));
    EXPECT_FALSE(comp(book4, book2));
}

TEST(ComparatorTest, LessByYear) {
    LessByYear comp;
    EXPECT_TRUE(comp(book3, book1));
    EXPECT_TRUE(comp(book1, book2));
    EXPECT_FALSE(comp(book2, book4));
}

TEST(ComparatorTest, LessByTitle) {
    LessByTitle comp;

    EXPECT_TRUE(comp(book3, book2));
    EXPECT_TRUE(comp(book2, book1));
    EXPECT_FALSE(comp(book2, book4));
}

TEST(ComparatorTest, LessByRating) {
    LessByRating comp;
    EXPECT_TRUE(comp(book2, book1));
    EXPECT_FALSE(comp(book2, book4));
}

TEST(ComparatorTest, LessByReadCount) {
    LessByReadCount comp;
    EXPECT_TRUE(comp(book1, book3));
    EXPECT_FALSE(comp(book2, book4));
}

TEST(ComparatorTest, LessByGenre) {
    LessByGenre comp;

    EXPECT_TRUE(comp(book1, book2));
    EXPECT_FALSE(comp(book2, book4));
}
