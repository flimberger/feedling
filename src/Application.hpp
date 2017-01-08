#ifndef FEEDLING_APPLICATION_HPP
#define FEEDLING_APPLICATION_HPP

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QUrl>  // needed by moc

#include <QtNetwork/QNetworkAccessManager>

#include "FeedsModel.hpp"

class QNetworkReply;

namespace feedling {

class EntriesModel;
class View;

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(std::unique_ptr<View> &&view, QObject *parent=nullptr);
    ~Application();

    Q_SLOT void init();
    Q_SLOT void onFetchFeeds();

    Q_INVOKABLE void setEntryList(QUrl url);

private:
    Q_SLOT void onFeedDownloadFinished(QNetworkReply *reply);

    void getFeedsFromConfig();

    QTimer m_timer;
    QNetworkAccessManager *m_network;
    std::unique_ptr<View> m_view;
    FeedsModel m_feedsModel;
    std::unique_ptr<EntriesModel> m_entriesModel;
};

}  // namespace feedling

#endif  // FEEDLING_APPLICATION_HPP
