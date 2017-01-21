#include "FeedsModel.hpp"

#include <algorithm>
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
        const auto *item = static_cast<TreeItem *>(index.internalPointer());
        switch (role) {
        case Qt::DisplayRole:
        case Roles::NAME:
            return item->name();
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

    TreeItem *parentItem;
    if (parent.isValid()) {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    } else {
        parentItem = const_cast<Folder *>(m_rootFolder.get());
    }
    if (parentItem->type() == TreeItem::Type::FOLDER) {
        auto *folder = static_cast<Folder *>(parentItem);
        if (row < folder->size()) {
            return createIndex(row, column, folder->getItem(row).get());
        }
    }
    return QModelIndex();
}

QModelIndex FeedsModel::parent(const QModelIndex &child) const
{
    if (child.isValid()) {
        const auto *item = static_cast<TreeItem *>(child.internalPointer());
        auto parent = item->folder().lock();
        if (parent && (parent != m_rootFolder)) {
            int row = -1;
            const auto &items = parent->items();
            const auto it = std::find_if(std::cbegin(items), std::cend(items),
                                         [item](const std::shared_ptr<TreeItem> &p) {
                    return p.get() == item;
            });
            if (it != std::cend(items)) {
                row = it - std::cbegin(items);
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

int FeedsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_rootFolder->size();
    }
    if (parent.column() == 0) {
        const auto *item = static_cast<TreeItem *>(parent.internalPointer());
        if (item->type() == TreeItem::Type::FOLDER) {
            return static_cast<const Folder *>(item)->size();
        }
    }
    return 0;
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
                case TreeItem::Type::FEED: {
                    const auto feed = std::static_pointer_cast<Feed>(item);
                    if (feed->url() == feedUrl) {
                        return feed;
                    }
                    break;
                }
                case TreeItem::Type::FOLDER: {
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
