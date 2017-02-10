#include <FeedParser.hpp>

#include <gtest/gtest.h>

#include <QtCore/QString>
#include <QtCore/QUrl>

#include <Feed.hpp>

using namespace feedling;

TEST(test_parser, test_basic_parsing) {
    FeedParser parser{std::make_shared<Feed>(QString::fromUtf8("test"), QString::fromUtf8("test"),
                                             QUrl("http://purplekraken.com/blog/rss")), nullptr};
}
