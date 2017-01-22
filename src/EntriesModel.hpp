#ifndef FEEDLING_ENTRIESMODEL_HPP
#define FEEDLING_ENTRIESMODEL_HPP

#include <memory>

#include <QtCore/QAbstractItemModel>

namespace feedling {

class Entry;
class Feed;

class EntriesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        CONTENT = Qt::UserRole + 1,
        DATETIME,
        TITLE
    };

    EntriesModel(QObject *parent=nullptr);
    virtual ~EntriesModel();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const override;

    void setFeed(const std::shared_ptr<Feed> &feed);
    std::shared_ptr<Entry> getEntry(const QModelIndex &idx) const;

private:
    std::shared_ptr<Feed> m_feed;
};

}  // feedling

#endif  // FEEDLING_ENTRIESMODEL_HPP
