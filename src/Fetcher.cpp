#include "Fetcher.hpp"

#include <string_view>
#include <memory>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "AtomParser.hpp"
#include "Feed.hpp"
#include "FeedsModel.hpp"
#include "RSSParser.hpp"

namespace feedling {
namespace {

constexpr bool starts_with(std::string_view str, std::string_view substr) {
    auto compare = &std::string_view::traits_type::compare;

    return str.size() >= substr.size() && compare(str.data(), substr.data(), substr.size()) == 0;
}

enum class FeedType {
    Indeterminate,  // application/xml, text/xml
    Atom,  // application/atom+xml
    RSS  // application/rss+xml
};

constexpr FeedType feedType(std::string_view contentType) {
    constexpr std::string_view ATOM = "application/atom+xml";
    constexpr std::string_view RSS = "application/rss+xml";

    if (starts_with(contentType, ATOM)) {
        return FeedType::Atom;
    } else if (starts_with(contentType, RSS)) {
        return FeedType::RSS;
    }

    return FeedType::Indeterminate;
}

}  // namespace

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
    qDebug() << "fetching" << m_feedsModel->size() << "feeds";

    // TODO: parallel jobs
    for (const auto &feed : *m_feedsModel) {
        // TODO: decide if this is the correct way, but it does indeed fix blog.qt.io
        auto request = QNetworkRequest{feed->url()};
        // TODO: should redirects be handled by the access manager?
        // TODO: should (permanent) redirects update the feed URL?
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        // TODO: extend "User-Agent" header with Qt version, OS and architecture
        request.setRawHeader("User-Agent", "feedling");  // wordpress.com requires this header
        // TODO: set "Accept" header
        // TODO: set "Accept-Charset" header
        m_network->get(request);
    }
}

void Fetcher::onFeedDownloadFinished(QNetworkReply *reply)
{
    const auto &url = reply->url();
    // TODO: I'm not shure this does belong into the fetcher, but another level of indirection seems
    // also like unneccessary overhead to me. Maybe it belongs into the Feed class.
    auto feed = m_feedsModel->getFeed(url);
    if (feed) {
        // TODO: this should be performed in a separate background thread
        auto contentType = reply->rawHeader("Content-Type");
        qDebug() << "Content-Type:" << contentType;
        auto type = feedType(contentType.data());
        std::unique_ptr<FeedParser> parser;

        switch (type) {
        case FeedType::Atom:
            parser = std::make_unique<AtomParser>(feed);
            break;
        case FeedType::RSS:
            parser = std::make_unique<RSSParser>(feed);
            break;
        default:
            qWarning() << "indeterminate content:" << url.toString();
        }

        if (parser) {
            if (!parser->read(reply)) {
                qWarning() << "failed to parse" << url.toString() << ':' << parser->errorString();
            }
        }
    } else {
        qWarning() << "unknown URL:" << url;
    }
}

}  // namespace feedling
