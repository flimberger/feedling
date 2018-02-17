#include "Entry.hpp"

#include "Feed.hpp"

namespace feedling {

Entry::Entry(QString title, QString content, QString id, const QDateTime &dateTime,
             const std::shared_ptr<Feed> &feed)
  : std::enable_shared_from_this<Entry>{},
    m_title(title),
    m_content(content),
    m_id{id},
    m_dateTime(dateTime),
    m_feed(feed)
{}

Entry::~Entry() = default;

QString Entry::content() const
{
    return m_content;
}
QString Entry::title() const
{
    return m_title;
}

const QDateTime &Entry::dateTime() const
{
    return m_dateTime;
}

std::weak_ptr<Feed> Entry::feed() const
{
    return m_feed;
}

}  // namespace feedling
