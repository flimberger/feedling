#ifndef FEEDLING_APPLICATION_HPP
#define FEEDLING_APPLICATION_HPP

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <QtNetwork/QNetworkAccessManager>

#include <QtQml/QQmlApplicationEngine>

#include "FeedsModel.hpp"

class QNetworkReply;

namespace feedling {

class EntriesModel;

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent=nullptr);
    ~Application();

    Q_SLOT void onCreated();
    Q_SLOT void onFetchFeeds();

    Q_INVOKABLE void setEntryList(QUrl url);

private:
    Q_SLOT void onFeedDownloadFinished(QNetworkReply *reply);

    void getFeedsFromConfig();

    QTimer m_timer;
    QNetworkAccessManager *m_network;
    QQmlApplicationEngine m_qmlAppEngine;
    FeedsModel m_feedsModel;
    std::unique_ptr<EntriesModel> m_entriesModel;
};

}  // namespace feedling

#endif  // FEEDLING_APPLICATION_HPP
