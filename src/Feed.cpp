#include "Feed.hpp"

#include <algorithm>

namespace feedling {

Feed::Feed(QString name, QString description, const QUrl &url)
  : m_name{name},
    m_description{description},
    m_url{url}
{}

QString Feed::name() const
{
    return m_name;
}

void Feed::setName(QString newName)
{
    m_name = newName;
}

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

int Feed::size() const
{
    return static_cast<int>(m_entries.size());
}

std::shared_ptr<Entry> Feed::getEntry(int idx) const
{
    Q_ASSERT(idx >= 0);
    return m_entries[static_cast<decltype (m_entries)::size_type>(idx)];
}

std::optional<const std::shared_ptr<Entry>> Feed::getEntry(QByteArray id) const
{
    auto iter = std::find_if(std::cbegin(m_entries), std::cend(m_entries),
                             [id](const std::shared_ptr<Entry> &entry) {
        return entry->id() == id;
    });

    if (iter == std::cend(m_entries)) {
        return std::nullopt;
    }

    return *iter;
}

void Feed::addEntry(std::shared_ptr<Entry> &&entry)
{
    m_entries.emplace_back(entry);
}

}  // namespace feedling
