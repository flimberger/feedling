#ifndef FEEDLING_FEED_H
#define FEEDLING_FEED_H

#include <optional>
#include <vector>

#include <QtCore/QUrl>

#include "Entry.hpp"

namespace feedling {

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

}  // namespace feedling

#endif // FEEDLING_FEED_H
