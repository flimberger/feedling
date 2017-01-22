#include "EntriesModel.hpp"

#include <climits>
#include <type_traits>

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QVector>

#include "Entry.hpp"
#include "Feed.hpp"

namespace feedling {

EntriesModel::EntriesModel(QObject *parent)
  : QAbstractListModel (parent)
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
        case Qt::DisplayRole:
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

    if ((row >= 0) && (row < m_feed->size())) {
        index = createIndex(row, column, m_feed->getEntry(row).get());
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

    return m_feed->size();
}

void EntriesModel::setFeed(const std::shared_ptr<Feed> &feed)
{
    m_feed = feed;
    emit dataChanged(index(0, 0, QModelIndex{}),
                     index(feed->size() - 1, 0, QModelIndex{}),
                     QVector<int>{Qt::DisplayRole, Roles::CONTENT, Roles::DATETIME, Roles::TITLE});
}

std::shared_ptr<Entry> EntriesModel::getEntry(const QModelIndex &idx) const
{
    if (idx.isValid()) {
        return static_cast<Entry *>(idx.internalPointer())->shared_from_this();
    }
    return std::shared_ptr<Entry>{};
}

}  // feedling
