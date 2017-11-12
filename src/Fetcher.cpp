#include "Fetcher.hpp"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "Feed.hpp"
#include "FeedParser.hpp"
#include "FeedsModel.hpp"

namespace feedling {

Fetcher::Fetcher(FeedsModel *model, QObject *parent)
  : QObject(parent),
    m_feedsModel{model},
    m_network{new QNetworkAccessManager(this)}
{}

void Fetcher::onFetch()
{
    const auto feeds = m_feedsModel->feeds();
    qDebug() << "fetching" << feeds.size() << "feeds";
    // TODO: parallel jobs
    for (const auto &wref : feeds) {
        if (const auto feed = wref.lock()) {
            onFeedDownloadFinished(m_network->get(QNetworkRequest(feed->url())));
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
        qDebug() << "Starting parser for " << url.toString();
        auto *parser = new FeedParser{feed, reply};
        QObject::connect(parser, &FeedParser::done,
                         [parser, reply](bool success, const std::shared_ptr<Feed> &feed) {
            qDebug() << feed->url().toString() << " is done " << success;
            delete parser;  // TODO: maybe there should be a registry...
            reply->deleteLater();
        });
    } else {
        qWarning() << "unknown URL:" << url;
    }
}

}  // namespace feedling
