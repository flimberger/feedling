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
    enum State {
        STATE_WAITING,
        STATE_SUCCEEDED,
        STATE_FAILED
    };

    explicit FeedParser(std::shared_ptr<Feed> feed);
    ~FeedParser();

    State parse();
    void addData(const QByteArray &data);
    State state() const;

private:
    std::unique_ptr<QXmlStreamReader> m_xmlReader;
    std::shared_ptr<Feed> m_feed;
    QString m_currentTag;
    State m_state;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
