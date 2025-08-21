#include "book_database.hpp"
#include <gtest/gtest.h>

using namespace bookdb;

TEST(BookDatabase, EmptyDB) {
    BookDatabase db;

    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.GetAuthors().empty());
    EXPECT_EQ(db.begin(), db.end());
}

TEST(BookDatabase, AddSingleBookWithEnumGenre) {
    BookDatabase db;
    auto &book = db.EmplaceBack("The Hobbit", "Tolkien", 1937, Genre::Fiction, 4.8, 1200);
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(book._title, "The Hobbit");
    EXPECT_EQ(book._genre, Genre::Fiction);
    EXPECT_EQ(db.GetAuthors().size(), 1);
    EXPECT_EQ(db.GetAuthors().count("Tolkien"), 1);
}

TEST(BookDatabase, AddSingleBookWithStringGenre) {
    BookDatabase db;
    auto &book = db.EmplaceBack("The Hobbit", "Tolkien", 1937, "Fiction", 4.8, 1200);
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(book._title, "The Hobbit");
    EXPECT_EQ(book._genre, Genre::Fiction);
    EXPECT_EQ(db.GetAuthors().size(), 1);
    EXPECT_EQ(db.GetAuthors().count("Tolkien"), 1);
}

TEST(BookDatabase, ClearDatabase) {
    BookDatabase db;
    db.EmplaceBack("Book1", "Author1", 2000, "SciFi", 4.5, 500);
    db.Clear();
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST(BookInsertionTest, EmplaceBack) {
    BookDatabase db;
    db.EmplaceBack("1984", "Orwell", 1949, "Fiction", 4.8, 2000);
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(db.begin()->_year, 1949);
}

TEST(BookInsertionTest, PushBackLvalue) {
    BookDatabase db;
    Book book("Dune", "Herbert", 1965, "SciFi", 4.7, 1500);
    db.push_back(book);
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(db.begin()->_title, "Dune");
}

TEST(BookInsertionTest, PushBackRvalue) {
    BookDatabase db;
    db.push_back(Book("Foundation", "Asimov", 1951, "SciFi", 4.6, 1800));
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(db.begin()->_author, "Asimov");
}

TEST(BookInsertionTest, InitializerList) {
    BookDatabase db = {{"Book1", "Author1", 2001, "Genre1", 3.5, 100}, {"Book2", "Author2", 2002, "Genre2", 4.0, 200}};
    EXPECT_EQ(db.size(), 2);
    EXPECT_EQ(db.GetAuthors().size(), 2);
}

TEST(AuthorManagementTest, AuthorInterning) {
    BookDatabase db;
    db.EmplaceBack("Book1", "Author", 2020, "Fiction", 4.0, 100);
    db.EmplaceBack("Book2", "Author", 2021, "NonFiction", 4.2, 150);

    const auto &books = db.GetBooks();
    EXPECT_EQ(books[0]._author.data(), books[1]._author.data());
}

TEST(AuthorManagementTest, MultipleAuthors) {
    BookDatabase db;
    db.EmplaceBack("Book1", "AuthorA", 2000, "Genre", 3.9, 50);
    db.EmplaceBack("Book2", "AuthorB", 2010, "Genre", 4.1, 70);
    db.EmplaceBack("Book3", "AuthorA", 2020, "Genre", 4.3, 90);

    EXPECT_EQ(db.GetAuthors().size(), 2);
    EXPECT_EQ(db.GetAuthors().count("AuthorA"), 1);
    EXPECT_EQ(db.GetAuthors().count("AuthorB"), 1);
}

TEST(IteratorTest, ForwardIteration) {
    BookDatabase db = {{"Book1", "Author1", 2000, "Genre1", 3.5, 100},
                       {"Book2", "Author2", 2010, "Genre2", 4.0, 200},
                       {"Book3", "Author3", 2020, "Genre3", 4.5, 300}};

    auto it = db.begin();
    EXPECT_EQ(it->_title, "Book1");
    EXPECT_EQ((++it)->_title, "Book2");
    EXPECT_EQ((++it)->_title, "Book3");
    EXPECT_EQ(++it, db.end());
}

TEST(IteratorTest, ReverseIteration) {
    BookDatabase db = {{"Book1", "Author1", 2000, "Genre1", 3.5, 100},
                       {"Book2", "Author2", 2010, "Genre2", 4.0, 200},
                       {"Book3", "Author3", 2020, "Genre3", 4.5, 300}};

    auto rit = db.rbegin();
    EXPECT_EQ(rit->_title, "Book3");
    EXPECT_EQ((++rit)->_title, "Book2");
    EXPECT_EQ((++rit)->_title, "Book1");
    EXPECT_EQ(++rit, db.rend());
}

TEST(IteratorTest, ConstIteration) {
    const BookDatabase db = {{"Book1", "Author1", 2000, "Genre1", 3.5, 100}};

    EXPECT_EQ(db.cbegin()->_title, "Book1");
    EXPECT_NE(db.cbegin(), db.cend());
}

TEST(GenreConversionTest, GenreFromStringValid) {
    EXPECT_EQ(GenreFromString("Fiction"), Genre::Fiction);
    EXPECT_EQ(GenreFromString("Mystery"), Genre::Mystery);
    EXPECT_EQ(GenreFromString("NonFiction"), Genre::NonFiction);
    EXPECT_EQ(GenreFromString("SciFi"), Genre::SciFi);
    EXPECT_EQ(GenreFromString("Biography"), Genre::Biography);
}

TEST(GenreConversionTest, GenreFromStringCaseSensitivity) {
    EXPECT_EQ(GenreFromString("fiction"), Genre::Unknown);
    EXPECT_EQ(GenreFromString("FICTION"), Genre::Unknown);
    EXPECT_EQ(GenreFromString("sCifi"), Genre::Unknown);
}

TEST(GenreConversionTest, GenreFromStringEdgeCases) {
    EXPECT_EQ(GenreFromString(""), Genre::Unknown);
    EXPECT_EQ(GenreFromString("Unknown"), Genre::Unknown);
    EXPECT_EQ(GenreFromString("Horror"), Genre::Unknown);
    EXPECT_EQ(GenreFromString("Fictio"), Genre::Unknown);
    EXPECT_EQ(GenreFromString("Fiction "), Genre::Unknown);
}

TEST(GenreConversionTest, GenreFromStringConstexpr) {
    constexpr Genre genre1 = GenreFromString("Fiction");
    constexpr Genre genre2 = GenreFromString("SciFi");
    constexpr Genre genre3 = GenreFromString("Invalid");

    static_assert(genre1 == Genre::Fiction, "Constexpr Fiction failed");
    static_assert(genre2 == Genre::SciFi, "Constexpr SciFi failed");
    static_assert(genre3 == Genre::Unknown, "Constexpr Unknown failed");
}

TEST(GenreConversionTest, GenreToStringValid) {
    EXPECT_EQ(GenreToString(Genre::Fiction), "Fiction");
    EXPECT_EQ(GenreToString(Genre::Mystery), "Mystery");
    EXPECT_EQ(GenreToString(Genre::NonFiction), "NonFiction");
    EXPECT_EQ(GenreToString(Genre::SciFi), "SciFi");
    EXPECT_EQ(GenreToString(Genre::Biography), "Biography");
}

TEST(GenreConversionTest, GenreToStringInvalid) {
    EXPECT_EQ(GenreToString(Genre::Unknown), "Unknown");
    EXPECT_EQ(GenreToString(static_cast<Genre>(-1)), "Unknown");
    EXPECT_EQ(GenreToString(static_cast<Genre>(100)), "Unknown");
}

TEST(GenreConversionTest, GenreToStringConstexpr) {
    constexpr auto str1 = GenreToString(Genre::Fiction);
    constexpr auto str2 = GenreToString(Genre::Mystery);
    constexpr auto str3 = GenreToString(Genre::Unknown);

    static_assert(std::string_view(str1) == "Fiction", "Constexpr Fiction failed");
    static_assert(std::string_view(str2) == "Mystery", "Constexpr Mystery failed");
    static_assert(std::string_view(str3) == "Unknown", "Constexpr Unknown failed");
}

TEST(GenreConversionTest, RoundTripConversion) {
    EXPECT_EQ(GenreFromString(GenreToString(Genre::Fiction)), Genre::Fiction);
    EXPECT_EQ(GenreFromString(GenreToString(Genre::Mystery)), Genre::Mystery);
    EXPECT_EQ(GenreFromString(GenreToString(Genre::NonFiction)), Genre::NonFiction);
    EXPECT_EQ(GenreFromString(GenreToString(Genre::SciFi)), Genre::SciFi);
    EXPECT_EQ(GenreFromString(GenreToString(Genre::Biography)), Genre::Biography);
    EXPECT_EQ(GenreFromString(GenreToString(Genre::Unknown)), Genre::Unknown);
}
