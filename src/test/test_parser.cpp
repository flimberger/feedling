#include <fstream>

#include <catch.hpp>

#include "../Feed.hpp"
#include "../FeedParser.hpp"

#include "test_data.hpp"

using namespace feedling;

TEST_CASE("Test the atom parser", "[Parser]")
{
    auto feed = std::make_shared<Feed>("test", "test feed", QUrl{"http://example.org/atom"});

    auto f = std::fstream{ATOM_TEST_PATH, std::fstream::in};
    if (!f.is_open()) {
        FAIL("failed to open test data \"" << ATOM_TEST_PATH << '\"');
    }
    auto data = std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    bool ret = FeedParser::parseXml(feed, data);

    REQUIRE(ret);
    REQUIRE(feed->size() == 2);
}
