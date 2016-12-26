#ifndef FEEDLING_FEEDPARSER_HPP
#define FEEDLING_FEEDPARSER_HPP

#include <memory>
#include <vector>

#include <QtCore/QString>

class QIODevice;
class QXmlStreamReader;

namespace feedling {

class Entry;
class Feed;

class FeedParser
{
public:
    explicit FeedParser(std::shared_ptr<Feed> feed, QIODevice *ioDevice);
    ~FeedParser();

    void parseXml();

private:
    std::unique_ptr<QXmlStreamReader> m_xmlReader;
    std::shared_ptr<Feed> m_feed;
    QString m_currentTag;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
