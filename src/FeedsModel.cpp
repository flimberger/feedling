#include "FeedsModel.hpp"

#include <algorithm>
#include <climits>
#include <deque>

#include <QtCore/QtDebug>
#include <QtCore/QVector>

#include "Feed.hpp"
#include "ModelTree.hpp"

namespace feedling {

FeedsModel::FeedsModel(QObject *parent)
  : QAbstractItemModel(parent),
    m_rootFolder(std::make_unique<Folder>("[root]"))
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
            return createIndex(row, column, folder->getItem(row));
        }
    }
    return QModelIndex();
}

QModelIndex FeedsModel::parent(const QModelIndex &child) const
{
    if (child.isValid()) {
        auto *item = static_cast<TreeItem *>(child.internalPointer());
        auto *parent = item->folder();

        if (parent && (parent != m_rootFolder.get())) {
            int row = -1;
            const auto it = std::find_if(std::cbegin(*parent), std::cend(*parent),
                                         [item](const std::unique_ptr<TreeItem> &p) {
                    return p.get() == item;
            });

            if (it != std::cend(*parent)) {
                row = it - std::cbegin(*parent);
            }

            return createIndex(row, 0, parent);
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

FeedsModel::iterator FeedsModel::begin() { return std::begin(m_feeds); }
FeedsModel::const_iterator FeedsModel::begin() const { return std::cbegin(m_feeds); }
FeedsModel::const_iterator FeedsModel::cbegin() const { return std::cbegin(m_feeds); }

FeedsModel::iterator FeedsModel::end() { return std::end(m_feeds); }
FeedsModel::const_iterator FeedsModel::end() const { return std::cend(m_feeds); }
FeedsModel::const_iterator FeedsModel::cend() const { return std::cend(m_feeds); }

std::shared_ptr<Feed> FeedsModel::getFeed(QUrl feedUrl)
{
    const auto end = std::cend(m_feeds);
    auto iter = std::find_if(std::cbegin(m_feeds), end,
                             [feedUrl](const std::shared_ptr<Feed> &feed) {
        return feedUrl == feed->url();
    });

    if (iter == end) {
        return nullptr;
    }

    return *iter;
}

std::shared_ptr<Feed> FeedsModel::getItem(const QModelIndex &index)
{
    if (index.isValid()) {
        auto *ptr = static_cast<TreeItem *>(index.internalPointer());

        if (ptr->type() == TreeItem::Type::FEED) {
            return static_cast<FeedItem *>(ptr)->data();
        }
    }

    return nullptr;
}

FeedItem *FeedsModel::addFeed(const std::shared_ptr<Feed> &feed, Folder *folder)
{
    return static_cast<FeedItem *>(addItem(std::make_unique<FeedItem>(feed), folder));
}

Folder *FeedsModel::addFolder(QString name, Folder *folder)
{
    return static_cast<Folder *>(addItem(std::make_unique<Folder>(name), folder));
}

TreeItem *FeedsModel::addItem(std::unique_ptr<TreeItem> item, Folder *folder)
{
    auto *ptr = item.get();

    if (item->type() == TreeItem::Type::FEED) {
        m_feeds.emplace_back(static_cast<FeedItem *>(ptr)->data());
    }
    if (!folder) {
        folder = m_rootFolder.get();
    }
    folder->addItem(std::move(item));

    return ptr;
}

}  // namespace feedling
