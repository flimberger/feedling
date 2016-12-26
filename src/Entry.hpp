#ifndef FEEDLING_ENTRY_HPP
#define FEEDLING_ENTRY_HPP

#include <memory>

#include <QtCore/QDateTime>
#include <QtCore/QString>

namespace feedling {

class Feed;

class Entry
{
public:
    Entry(QString title, QString content, const QDateTime &dateTime,
          const std::shared_ptr<Feed> &feed);
    ~Entry();

    QString content() const;
    QString title() const;
    const QDateTime &dateTime() const;
    std::weak_ptr<Feed> feed() const;

private:
    QString m_title;
    QString m_content;
    QDateTime m_dateTime;
    std::weak_ptr<Feed> m_feed;
};

}  // namespace feedling

#endif  // FEEDLING_ENTRY_HPP