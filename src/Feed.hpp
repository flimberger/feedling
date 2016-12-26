#ifndef FEEDLING_FEED_H
#define FEEDLING_FEED_H

#include <vector>

#include <QtCore/QUrl>

#include "FeedsModelItem.hpp"
#include "Entry.hpp"

namespace feedling {

class Folder;

class Feed : public FeedsModelItem
{
public:
    explicit Feed(QString name, QString description, const QUrl &url);

    QString description() const;
    void setDescription(QString newDescription);

    const QUrl &url() const;
    void setUrl(const QUrl &newUrl);

    const std::vector<Entry> &entries() const;

    void addEntry(Entry &&entry);

private:
    QString m_description;
    QUrl m_url;
    std::vector<Entry> m_entries;
};

}  // namespace feedling

#endif // FEEDLING_FEED_H
