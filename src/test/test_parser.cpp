#include <catch.hpp>

#include <QtCore/QFile>

#include "../Feed.hpp"
#include "../AtomParser.hpp"
#include "../RSSParser.hpp"

#include "test_data.hpp"

using namespace feedling;

TEST_CASE("Test the Atom parser", "[Atom] [Parser]")
{
    auto feed = std::make_shared<Feed>("test", "test feed", QUrl{"http://example.org/atom"});

    QFile f{ATOM_TEST_PATH};

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        FAIL("Failed to open file \"" << ATOM_TEST_PATH << '\"');
    }

    AtomParser p{feed};

    bool ret = p.read(&f);

    if (!ret) {
        FAIL(p.errorString().toStdString());
    }

    REQUIRE(feed->size() == 2);
}

TEST_CASE("Test the RSS parser", "[RSS] [Parser]")
{
    auto feed = std::make_shared<Feed>("test", "test feed", QUrl{"http://example.com/rss"});

    QFile f{RSS_TEST_PATH};

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        FAIL("Failed to open file \"" << RSS_TEST_PATH << '\"');
    }

    RSSParser p{feed};

    bool ret = p.read(&f);

    if (!ret) {
        FAIL(p.errorString().toStdString());
    }

    REQUIRE(feed->size() == 2);
}
