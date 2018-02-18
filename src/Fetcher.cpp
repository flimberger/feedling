#include "Fetcher.hpp"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "AtomParser.hpp"
#include "Feed.hpp"
#include "FeedsModel.hpp"

namespace feedling {

Fetcher::Fetcher(FeedsModel *model, QObject *parent)
  : QObject(parent),
    m_feedsModel{model},
    m_network{new QNetworkAccessManager(this)}
{
    QObject::connect(m_network, &QNetworkAccessManager::finished,
                     this,      &Fetcher::onFeedDownloadFinished);
}

void Fetcher::onFetch()
{
    const auto feeds = m_feedsModel->feeds();
    qDebug() << "fetching" << feeds.size() << "feeds";
    // TODO: parallel jobs
    for (const auto &wref : feeds) {
        if (const auto feed = wref.lock()) {
            m_network->get(QNetworkRequest(feed->url()));
        }
    }
}

void Fetcher::onFeedDownloadFinished(QNetworkReply *reply)
{
    const auto &url = reply->url();
    // TODO: I'm not shure this does belong into the fetcher, but another level of indirection seems
    // also like unneccessary overhead to me. Maybe it belongs into the Feed class.
    auto feed = m_feedsModel->getFeed(url).lock();
    if (feed) {
        // TODO: this should be performed in a separate background thread
        AtomParser parser{feed};
        if (!parser.read(reply)) {
            qWarning() << "failed to parse" << url.toString() << ':' << parser.errorString();
        }
    } else {
        qWarning() << "unknown URL:" << url;
    }
}

}  // namespace feedling
