#ifndef FEEDLING_FEEDFETCHER_HPP
#define FEEDLING_FEEDFETCHER_HPP

#include <QtCore/QObject>

class QNetworkAccessManager;
class QNetworkReply;

namespace feedling {

class FeedsModel;

// Donwload the feeds provided in the model.
class Fetcher : public QObject
{
    Q_OBJECT

public:
    // Provide a model containing feeds to the fetcher.
    explicit Fetcher(FeedsModel *model, QObject *parent=nullptr);

    // Slot to fetch all feeds in the model.
    Q_SLOT void onFetch();

private:
    Q_SLOT void onFeedDownloadFinished(QNetworkReply *reply);

    FeedsModel *m_feedsModel;
    QNetworkAccessManager *m_network;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDFETCHER_HPP
