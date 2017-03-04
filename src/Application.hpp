#ifndef FEEDLING_APPLICATION_HPP
#define FEEDLING_APPLICATION_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QUrl>  // needed by moc

#include <QtNetwork/QNetworkAccessManager>

#include "EntriesModel.hpp"
#include "FeedsModel.hpp"
#include "Presenter.hpp"

class QNetworkReply;

namespace feedling {

class IStorage;
class View;

class Application : public QObject, Presenter
{
    Q_OBJECT
public:
    explicit Application(std::unique_ptr<View> &&view, std::unique_ptr<IStorage> &&storage,
                         QObject *parent=nullptr);
    ~Application();

    Q_SLOT void init();
    Q_SLOT void onFetchFeeds();

    void selectFeed(const QModelIndex &index) override;
    void selectEntry(const QModelIndex &index) override;

private:
    void parseFeed(QNetworkReply *reply);

    void getFeedsFromConfig();

    QTimer m_timer;
    QNetworkAccessManager *m_network;
    std::unique_ptr<View> m_view;
    std::unique_ptr<IStorage> m_storage;
    FeedsModel m_feedsModel;
    EntriesModel m_entriesModel;
};

}  // namespace feedling

#endif  // FEEDLING_APPLICATION_HPP
