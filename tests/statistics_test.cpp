#include "book_database.hpp"
#include "statistics.hpp"
#include <gtest/gtest.h>
#include <unordered_set>

using namespace bookdb;

static BookDatabase<> db{{"Book1", "AuthorA", 2000, "Fiction", 4.0, 100},
                         {"Book2", "AuthorB", 2010, "SciFi", 4.5, 200},
                         {"Book3", "AuthorA", 2020, "Fiction", 5.0, 300},
                         {"Book4", "AuthorC", 2005, "Mystery", 3.5, 150},
                         {"Book5", "AuthorA", 2015, "SciFi", 4.2, 250}};

TEST(StatisticsTest, AuthorHistogram) {
    auto hist = buildAuthorHistogramFlat(db);

    EXPECT_EQ(hist.size(), 3);
    EXPECT_EQ(hist["AuthorA"], 3);
    EXPECT_EQ(hist["AuthorB"], 1);
    EXPECT_EQ(hist["AuthorC"], 1);

    std::string_view author_key = "AuthorA";
    EXPECT_EQ(hist.find(author_key)->second, 3);

    EXPECT_EQ(hist.find("Unknown"), hist.end());
}

TEST(StatisticsTest, AverageRating) {
    double avg = calculateAverageRating(db);

    EXPECT_DOUBLE_EQ(avg, 4.24);

    BookDatabase<> empty_db;
    EXPECT_DOUBLE_EQ(calculateAverageRating(empty_db), 0.0);
}

TEST(StatisticsTest, GenreRatings) {
    auto genre_ratings = calculateGenreRatings(db.begin(), db.end());

    // Expected values:
    // Fiction: (4.0 + 5.0)/2 = 4.5
    // SciFi: (4.5 + 4.2)/2 = 4.35
    // Mystery: 3.5/1 = 3.5

    ASSERT_EQ(genre_ratings.size(), 3);
    EXPECT_DOUBLE_EQ(genre_ratings[Genre::Fiction], 4.5);
    EXPECT_DOUBLE_EQ(genre_ratings[Genre::SciFi], 4.35);
    EXPECT_DOUBLE_EQ(genre_ratings[Genre::Mystery], 3.5);

    EXPECT_EQ(genre_ratings.find(Genre::Biography), genre_ratings.end());
}

TEST(StatisticsTest, TopNByRating) {
    auto comp = [](const Book &a, const Book &b) { return a._rating < b._rating; };

    auto top_books = getTopNBy(db, 3, comp);

    ASSERT_EQ(top_books.size(), 3);
    EXPECT_EQ(top_books[0].get()._title, "Book3");
    EXPECT_EQ(top_books[1].get()._title, "Book2");
    EXPECT_EQ(top_books[2].get()._title, "Book5");

    auto all_books = getTopNBy(db, 10, comp);
    EXPECT_EQ(all_books.size(), 5);
}

TEST(StatisticsTest, RandomSample) {

    auto random_sample = sampleRandomBooks(db, 3);
    ASSERT_EQ(random_sample.size(), 3);

    // Sample books valid check
    for (const auto &book_ref : random_sample) {
        auto it = std::find_if(db.cbegin(), db.cend(), [&](const Book &b) { return &b == &book_ref.get(); });
        EXPECT_NE(it, db.cend());
    }

    // Sample books unique check
    std::unordered_set<const Book *> unique_books;
    for (const auto &book_ref : random_sample) {
        unique_books.insert(&book_ref.get());
    }
    EXPECT_EQ(unique_books.size(), random_sample.size());

    auto full_sample = sampleRandomBooks(db, 10);
    EXPECT_EQ(full_sample.size(), 5);
}

TEST(StatisticsTest, EmptyDatabase) {
    BookDatabase<> empty_db;

    auto hist = buildAuthorHistogramFlat(empty_db);
    EXPECT_TRUE(hist.empty());

    EXPECT_DOUBLE_EQ(calculateAverageRating(empty_db), 0.0);

    auto genre_ratings = calculateGenreRatings(empty_db.begin(), empty_db.end());
    EXPECT_TRUE(genre_ratings.empty());

    auto comp = [](const Book &a, const Book &b) { return a._rating > b._rating; };
    auto top_books = getTopNBy(empty_db, 3, comp);
    EXPECT_TRUE(top_books.empty());

    auto random_sample = sampleRandomBooks(empty_db, 3);
    EXPECT_TRUE(random_sample.empty());
}
