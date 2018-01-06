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

Q_SIGNALS:
    void done(bool success, const std::shared_ptr<Feed> &feed);

private:
    Q_SLOT void onDataReady();
    void parseXml();

    std::unique_ptr<QXmlStreamReader> m_xmlReader;
    std::shared_ptr<Feed> m_feed;
    QIODevice *m_ioDevice;
    QString m_currentTag;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
