#include "Feed.hpp"

#include "Folder.hpp"

namespace feedling {

Feed::Feed(QString name, QString description, const QUrl &url)
  : feedling::TreeItem (name, TreeItem::Type::FEED),
    m_description(description),
    m_url(url)
{}

QString Feed::description() const
{
    return m_description;
}

void Feed::setDescription(QString newDescription)
{
    m_description = newDescription;
}

const QUrl &Feed::url() const
{
    return m_url;
}

void Feed::setUrl(const QUrl &newUrl)
{
    m_url = newUrl;
}

const std::vector<Entry> &Feed::entries() const
{
    return m_entries;
}

void Feed::addEntry(Entry &&entry)
{
    m_entries.emplace_back(entry);
}

}  // namespace feedling
