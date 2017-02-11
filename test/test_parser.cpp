#include <FeedParser.hpp>

#include <gtest/gtest.h>

#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

#include <Entry.hpp>
#include <Feed.hpp>

using namespace feedling;

namespace {

inline const char *str(const QString &qstr)
{
    return qstr.toUtf8().constData();
}

class TestParser : public ::testing::Test
{
public:
    TestParser();

protected:
    std::shared_ptr<Feed> m_feed;
    FeedParser m_parser;
};

TestParser::TestParser()
  : m_feed{std::make_shared<Feed>(QString::fromUtf8("test"), QString::fromUtf8("test"),
                                  QUrl("http://purplekraken.com/blog/rss"))},
    m_parser{m_feed}
{}

}  // namespace

TEST_F(TestParser, TestBasicParsing) {
    auto data = QByteArray{
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<rss version=\"2.0\" xmlns:content=\"http://purl.org/rss/1.0/modules/content/\">"
            "<channel>"
            "<item><title>Simple Title</title><pubDate>2017-02-10T22:51</pubDate>"
            "<link>http://purplekraken.com/blog/2017/02/10/entry</link>"
            "<description>A simple test entry</description>"
            "<content:encoded>A simple test entry</content:encoded></item>"
            "</channel></rss>"
    };
    m_parser.addData(data);
    const auto state = m_parser.parse();
    ASSERT_EQ(state, FeedParser::STATE_SUCCEEDED);
    const auto entry = m_feed->getEntry(0);
    ASSERT_STREQ(str(entry->title()), "Simple Title");
    // TODO: figure out date parsing
    // ASSERT_STREQ(str(entry->dateTime().toString()), "2017-02-10T22:51");
    ASSERT_STREQ(str(entry->content()), "A simple test entry");
}

{
}
