#ifndef FEEDLING_FEEDPARSER_HPP
#define FEEDLING_FEEDPARSER_HPP

#include <memory>

#include <QtCore/QString>

class QIODevice;

namespace feedling {

class Feed;

// Common abstract base class for different feed parser implementations.
class FeedParser
{
public:
    virtual ~FeedParser();

    virtual bool read(QIODevice *ioDevice) = 0;

    virtual QString errorString() const = 0;

protected:
    FeedParser(const std::shared_ptr<Feed> &feed);

    const std::shared_ptr<Feed> &feed() const { return m_feed; }

private:
    std::shared_ptr<Feed> m_feed;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
