#ifndef FEEDLING_APPLICATION_HPP
#define FEEDLING_APPLICATION_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <QtNetwork/QNetworkAccessManager>

#include "EntriesModel.hpp"
#include "FeedsModel.hpp"
#include "Presenter.hpp"
#include "TextFileDataStore.h"

class QNetworkReply;

namespace feedling {

class View;

class Application : public QObject, Presenter
{
    Q_OBJECT
public:
    explicit Application(std::unique_ptr<View> &&view, QObject *parent=nullptr);
    ~Application();

    Q_SLOT void init();
    Q_SLOT void onFetchFeeds();

    void selectFeed(const QModelIndex &index) override;
    void selectEntry(const QModelIndex &index) override;

private:
    void parseFeed(QNetworkReply *reply);

    QTimer m_timer;
    QNetworkAccessManager *m_network;
    std::unique_ptr<View> m_view;
    EntriesModel m_entriesModel;
    FeedsModel m_feedsModel;
    TextFileDataStore m_dataStore;
};

}  // namespace feedling

#endif  // FEEDLING_APPLICATION_HPP
