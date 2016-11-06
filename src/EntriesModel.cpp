#include "EntriesModel.hpp"

#include <climits>
#include <type_traits>

#include <QtCore/QByteArray>
#include <QtCore/QHash>

#include "Entry.hpp"
#include "Feed.hpp"

namespace feedling {

EntriesModel::EntriesModel(std::shared_ptr<Feed> feed, QObject *parent)
  : QAbstractListModel (parent),
    m_feed(feed)
{}

EntriesModel::~EntriesModel() = default;

QVariant EntriesModel::data(const QModelIndex &index, int role) const
{
    auto data = QVariant();
    if (index.isValid()) {
        const auto *entry = static_cast<const Entry *>(index.internalPointer());
        switch (role) {
        case Roles::CONTENT:
            data.setValue<QString>(entry->content());
            break;
        case Roles::DATETIME:
            data.setValue<QDateTime>(entry->dateTime());
            break;
        case Roles::TITLE:
            data.setValue<QString>(entry->title());
            break;
        }
    }
    return data;
}

QModelIndex EntriesModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_UNUSED(column);

    auto index = QModelIndex();
    const auto &entries = m_feed->entries();
    const auto idx = static_cast<std::vector<Entry>::size_type>(row);

    if (idx < entries.size()) {
        index = createIndex(row, column, const_cast<Entry *>(&entries[idx]));
    }

    return index;
}

QHash<int, QByteArray> EntriesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Roles::CONTENT] = "content";
    roles[Roles::DATETIME] = "dateTime";
    roles[Roles::TITLE] = "title";

    return roles;
}

int EntriesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_ASSERT(m_feed->entries().size() < INT_MAX);

    return static_cast<int>(m_feed->entries().size());
}

}  // feedling
