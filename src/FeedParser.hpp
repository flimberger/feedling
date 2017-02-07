#ifndef FEEDLING_FEEDPARSER_HPP
#define FEEDLING_FEEDPARSER_HPP

#include <memory>
#include <vector>

#include <QtCore/QObject>
#include <QtCore/QString>

class QIODevice;
class QXmlStreamReader;

namespace feedling {

class Entry;
class Feed;

class FeedParser : public QObject
{
    Q_OBJECT

public:
    explicit FeedParser(std::shared_ptr<Feed> feed, QIODevice *ioDevice, QObject *parent=nullptr);
    ~FeedParser();

    void parseXml();

Q_SIGNALS:
    void done(bool success, const std::shared_ptr<Feed> &feed);

private:
    std::unique_ptr<QXmlStreamReader> m_xmlReader;
    std::shared_ptr<Feed> m_feed;
    QString m_currentTag;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
