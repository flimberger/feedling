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

namespace {

inline const char *str(const QString &qstr)
{
    return qstr.toUtf8().constData();
}

}  // namespace

using namespace feedling;

TEST(test_parser, test_basic_parsing) {
    auto feed = std::make_shared<Feed>(QString::fromUtf8("test"), QString::fromUtf8("test"),
                                       QUrl("http://purplekraken.com/blog/rss"));
    auto data = QByteArray{
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?><rss version=\"2.0\"><channel>"
            "<item><title>Simple Title</title><pubDate>2017-02-10T22:51</pubDate>"
            "<link>http://purplekraken.com/blog/2017/02/10/entry</link>"
            "<description>A simple test entry</description>"
            "<content:encoded>A simple test entry</content:encoded></item>"
            "</channel></rss>"
    };
    QBuffer buffer{};
    FeedParser parser{feed, &buffer};
    bool parser_success = false;
    QObject::connect(&parser, &FeedParser::done,
                     [&parser_success](bool success, const std::shared_ptr<Feed> &feed) {
        parser_success = success;
    });
    QTimer::singleShot(0, [&data, &buffer]() { buffer.setData(data); });
    ASSERT_TRUE(parser_success);
    ASSERT_EQ(feed->size(), 1);
    const auto entry = feed->getEntry(0);
    ASSERT_STREQ(str(entry->title()), "Simple Title");
    ASSERT_STREQ(str(entry->dateTime().toString()), "2017-02-10T22:51");
    ASSERT_STREQ(str(entry->content()), "A simple test entry");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    QCoreApplication app{argc, argv};
    RUN_ALL_TESTS();
    QTimer::singleShot(0, []() { qApp->exit(); });
    return app.exec();
}
