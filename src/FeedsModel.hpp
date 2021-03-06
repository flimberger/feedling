#ifndef FEEDLING_FEEDMODEL_HPP
#define FEEDLING_FEEDMODEL_HPP

#include <iterator>
#include <memory>
#include <vector>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

namespace feedling {

class Feed;
class FeedItem;
class Folder;
class TreeItem;

class FeedsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    using value_type = std::shared_ptr<Feed>;
    using container_type = std::vector<value_type>;
    using size_type = container_type::size_type;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;

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
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;


    // FeedsModel interface

    bool is_empty() const noexcept;  // Contains feeds?
    size_type size() const noexcept;  // Number of feeds

    // Iterators for the feeds
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;


    std::shared_ptr<Feed> getFeed(QUrl feedUrl);

    std::shared_ptr<Feed> getItem(const QModelIndex &index);

    FeedItem *addFeed(const std::shared_ptr<Feed> &item, Folder *folder);
    Folder *addFolder(QString name, Folder *folder);

private:
    TreeItem *addItem(std::unique_ptr<TreeItem> item, Folder *folder);

    std::unique_ptr<Folder> m_rootFolder;
    container_type m_feeds;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDMODEL_HPP
