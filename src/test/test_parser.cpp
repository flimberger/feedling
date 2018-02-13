#include <catch.hpp>

#include <QtCore/QFile>

#include "../Feed.hpp"
#include "../FeedParser.hpp"

#include "test_data.hpp"

using namespace feedling;

TEST_CASE("Test the atom parser", "[Parser]")
{
    auto feed = std::make_shared<Feed>("test", "test feed", QUrl{"http://example.org/atom"});

    QFile f{ATOM_TEST_PATH};

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        FAIL("Failed to open file \"" << ATOM_TEST_PATH << '\"');
    }

    FeedParser p{feed};

    bool ret = p.read(&f);

    if (!ret) {
        FAIL(p.errorString().toStdString());
    }

    REQUIRE(feed->size() == 2);
}
