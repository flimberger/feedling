#ifndef FEEDLING_FEED_H
#define FEEDLING_FEED_H

#include <optional>
#include <vector>

#include <QtCore/QUrl>

#include "TreeItem.hpp"
#include "Entry.hpp"

namespace feedling {

class Folder;

class Feed
{
public:
    explicit Feed(QString name, QString description, const QUrl &url);

    QString name() const;
    void setName(QString newName);

    QString description() const;
    void setDescription(QString newDescription);

    const QUrl &url() const;
    void setUrl(const QUrl &newUrl);

    int size() const;
    std::shared_ptr<Entry> getEntry(int idx) const;

    std::optional<const std::shared_ptr<Entry>> getEntry(QByteArray id) const;
    void addEntry(std::shared_ptr<Entry> &&entry);

private:
    QString m_name;
    QString m_description;
    QUrl m_url;
    std::vector<std::shared_ptr<Entry>> m_entries;
};

class FeedItem : public TreeItem {
public:
    explicit FeedItem(const std::shared_ptr<Feed> &feed);

    const Feed *data() const;
    std::shared_ptr<Feed> data();

private:
    std::shared_ptr<Feed> m_feed;
};

}  // namespace feedling

#endif // FEEDLING_FEED_H
