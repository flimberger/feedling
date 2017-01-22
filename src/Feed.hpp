#ifndef FEEDLING_FEED_H
#define FEEDLING_FEED_H

#include <vector>

#include <QtCore/QUrl>

#include "TreeItem.hpp"
#include "Entry.hpp"

namespace feedling {

class Folder;

class Feed : public TreeItem
{
public:
    explicit Feed(QString name, QString description, const QUrl &url);

    QString description() const;
    void setDescription(QString newDescription);

    const QUrl &url() const;
    void setUrl(const QUrl &newUrl);

    int size() const;
    std::shared_ptr<Entry> getEntry(int idx) const;

    void addEntry(std::shared_ptr<Entry> &&entry);

private:
    QString m_description;
    QUrl m_url;
    std::vector<std::shared_ptr<Entry>> m_entries;
};

}  // namespace feedling

#endif // FEEDLING_FEED_H
