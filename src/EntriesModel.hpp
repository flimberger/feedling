#ifndef FEEDLING_ENTRIESMODEL_HPP
#define FEEDLING_ENTRIESMODEL_HPP

#include <memory>

#include <QtCore/QAbstractItemModel>

namespace feedling {

class Feed;

class EntriesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        CONTENT,
        DATETIME,
        TITLE
    };

    EntriesModel(std::shared_ptr<Feed> feed, QObject *parent=nullptr);
    virtual ~EntriesModel();

    virtual QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const override;

private:
    std::shared_ptr<Feed> m_feed;
};

}  // feedling

#endif  // FEEDLING_ENTRIESMODEL_HPP
