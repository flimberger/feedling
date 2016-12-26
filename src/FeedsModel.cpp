#include "FeedsModel.hpp"

#include <climits>
#include <deque>

#include <QtCore/QtDebug>
#include <QtCore/QVector>

#include "Feed.hpp"

namespace feedling {

FeedsModel::FeedsModel(QObject *parent)
  : QAbstractItemModel(parent),
    m_rootFolder(std::make_shared<Folder>("[root]"))
{}

FeedsModel::~FeedsModel() = default;

int FeedsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}


// QAbstractItemModel interface

QVariant FeedsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        const auto *item = static_cast<const FeedsModelItem *>(index.internalPointer());
        if (role == Roles::NAME) {
            return item->name();
        } else if (role == Roles::TYPE) {
            return static_cast<int>(item->type());
        } else {
            if (item->type() == FeedsModelItem::Type::FEED) {
                const auto *feed = static_cast<const Feed *>(index.internalPointer());
                if (role == Roles::DESCRIPTION) {
                    return feed->description();
                } else if (role == Roles::URL) {
                    return feed->url();
                }
            }
        }
    }
    return QVariant();
}

QVariant FeedsModel::headerData(int section, Qt::Orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Description");
        case 2:
            return tr("URL");
        default:
            return tr("test");
        }
    }
    return QVariant();
}

QModelIndex FeedsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    FeedsModelItem *parentItem;
    if (parent.isValid()) {
        parentItem = static_cast<FeedsModelItem *>(parent.internalPointer());
    } else {
        parentItem = const_cast<Folder *>(m_rootFolder.get());
    }
    switch (parentItem->type()) {
        case FeedsModelItem::Type::FEED: {
            auto *feed = static_cast<Feed *>(parentItem);
            const auto &entries = feed->entries();
            Q_ASSERT(entries.size() != INT_MAX);
            if (row < static_cast<int>(entries.size())) {
                return createIndex(row, column, const_cast<Entry *>(&entries[row]));
            }
            break;
        }
        case FeedsModelItem::Type::FOLDER: {
            auto *folder = static_cast<Folder *>(parentItem);
            if (row < folder->size()) {
                return createIndex(row, column, folder->items()[row].get());
            }
            break;
        }
    }
    return QModelIndex();
}

QModelIndex FeedsModel::parent(const QModelIndex &index) const
{
    if (index.isValid()) {
        auto item = static_cast<FeedsModelItem *>(index.internalPointer());
        auto parent = item->folder().lock();
        if (parent && (parent != m_rootFolder)) {
            int row = 0;
            for (const auto &i : parent->items()) {
                if (i->name() == item->name()) {
                    break;
                }
                row++;
            }
            return createIndex(row, 0, parent.get());
        }
    }
    return QModelIndex();
}

QHash<int, QByteArray> FeedsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Roles::NAME] = "name";
    roles[Roles::DESCRIPTION] = "description";
    roles[Roles::TYPE] = "type";
    roles[Roles::URL] = "url";

    return roles;
}

int FeedsModel::rowCount(const QModelIndex &index) const
{
    if (index.column() > 0) {
        return 0;
    }

    int count = 0;

    FeedsModelItem *item;
    if (index.isValid()) {
        item = static_cast<FeedsModelItem *>(index.internalPointer());
    } else {
        item = const_cast<Folder *>(m_rootFolder.get());
    }
    if (item->type() == FeedsModelItem::Type::FOLDER) {
        count = static_cast<Folder *>(item)->size();
    }
    return count;
}


// FeedsModel interface

std::weak_ptr<Feed> FeedsModel::getFeed(QUrl feedUrl)
{
    auto folders = std::deque<std::shared_ptr<Folder>>{ m_rootFolder };
    while (!folders.empty()) {
        auto currentFolder = folders.front();
        folders.pop_front();

        for (const auto &item : currentFolder->items()) {
            switch (item->type()) {
                case FeedsModelItem::Type::FEED: {
                    const auto feed = std::static_pointer_cast<Feed>(item);
                    if (feed->url() == feedUrl) {
                        return feed;
                    }
                    break;
                }
                case FeedsModelItem::Type::FOLDER: {
                    folders.push_back(std::static_pointer_cast<Folder>(item));
                    break;
                }
            }
        }
    }
    return std::weak_ptr<Feed>();
}

const std::vector<std::weak_ptr<Feed>> &FeedsModel::feeds() const
{
    return m_feeds;
}

}  // namespace feedling
