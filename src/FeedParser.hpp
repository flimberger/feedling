#ifndef FEEDLING_FEEDPARSER_HPP
#define FEEDLING_FEEDPARSER_HPP

#include <memory>

#include <QtCore/QXmlStreamReader>

class QIODevice;

namespace feedling {

class Feed;

// Simple parser which currently only understands Atom
class FeedParser
{
public:
    FeedParser(const std::shared_ptr<Feed> &m_feed);
    ~FeedParser();

    bool read(QIODevice *ioDevice);

    QString errorString() const;

private:
    void readEntry();
    void readFeed();

    QXmlStreamReader m_xmlReader;

    std::shared_ptr<Feed> m_feed;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
