#include <catch.hpp>

#include <QtCore/QFile>

#include "../Feed.hpp"
#include "../FeedParser.hpp"

using namespace feedling;

TEST_CASE("Test the atom parser", "[Parser]")
{
    auto feed = std::make_shared<Feed>("test", "test feed", QUrl{"http://example.org/atom"});
    QFile reader{"atom_test.xml"};
    FeedParser parser{feed, &reader};

    REQUIRE(feed->size() == 2);
}
