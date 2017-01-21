#ifndef FEEDLING_FEEDMODEL_HPP
#define FEEDLING_FEEDMODEL_HPP

#include <iterator>
#include <memory>
#include <vector>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

#include "Folder.hpp"

namespace feedling {

class Feed;

class FeedsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles {
        NAME = Qt::UserRole + 1,
        DESCRIPTION,
        TYPE,
        URL
    };

    FeedsModel(QObject *parent=nullptr);
    ~FeedsModel();


    // QAbstractItemModel interface

    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role=Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;


    // FeedsModel interface

    std::weak_ptr<Feed> getFeed(QUrl feedUrl);
    const std::vector<std::weak_ptr<Feed>> &feeds() const;

    template<typename Container>
    std::weak_ptr<Folder> getFolder(const Container &path);

    template<typename Container>
    bool addItem(const std::shared_ptr<TreeItem> &item, const Container &path);

private:
    std::shared_ptr<Folder> m_rootFolder;
    std::vector<std::weak_ptr<Feed>> m_feeds;
};

template<typename Container>
std::weak_ptr<Folder> FeedsModel::getFolder(const Container &path)
{
    if (path.empty()) {
        return m_rootFolder;
    }
    auto currentFolder = m_rootFolder;
    for (const auto &name : path) {
        auto subFolder = currentFolder->getItem(name);
        if (!subFolder || (subFolder->type() != TreeItem::Type::FOLDER)) {
            break;
        } else {
            currentFolder = std::static_pointer_cast<Folder>(subFolder);
        }
    }
    return currentFolder;
}

template<typename Container>
bool FeedsModel::addItem(const std::shared_ptr<TreeItem> &item,
                         const Container &path)
{
    auto folder = getFolder(path).lock();
    if (folder) {
        folder->addItem(item);
        if (item->type() == TreeItem::Type::FEED) {
            const auto ptr = std::static_pointer_cast<Feed>(item);
            m_feeds.emplace_back(ptr);
        }
        return true;
    }
    return false;
}

}  // namespace feedling

#endif  // FEEDLING_FEEDMODEL_HPP
