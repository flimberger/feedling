#include "Application.hpp"

#include <iterator>
#include <vector>
#include <utility>

#include <QtCore/QtDebug>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtQml/QQmlContext>

#include "EntriesModel.hpp"
#include "Feed.hpp"
#include "FeedParser.hpp"
#include "View.hpp"

namespace feedling {

Application::Application(std::unique_ptr<View> &&view, QObject *parent)
  : QObject{parent},
    Presenter{},
    m_network{new QNetworkAccessManager(this)},
    m_view{std::move(view)},
    m_dataStore{".", &m_feedsModel}
{
    connect(m_network, &QNetworkAccessManager::finished,
            this,      &Application::parseFeed);
    init();
}

Application::~Application() = default;

void Application::init()
{
    m_view->init(this);
    m_view->setFeedsModel(&m_feedsModel);
    m_view->setEntriesModel(&m_entriesModel);
    // TODO: make this asynchronous
    m_dataStore.fetchFeeds();
}

void Application::onFetchFeeds()
{
    for (const auto &wref : m_feedsModel.feeds()) {
        if (const auto feed = wref.lock()) {
            parseFeed(m_network->get(QNetworkRequest(feed->url())));
        }
    }
}

void Application::selectFeed(const QModelIndex &index)
{
    auto feed = m_feedsModel.getItem(index);
    if (feed) {
        m_entriesModel.setFeed(feed);
        qDebug() << feed->name() << " selected";
    }
}

void Application::selectEntry(const QModelIndex &index)
{
    const auto &entry = m_entriesModel.getEntry(index);
    if (entry) {
        m_view->showEntry(entry);
        qDebug() << entry->title() << " selected";
    }
}

void Application::parseFeed(QNetworkReply *reply)
{
    const auto &url = reply->url();
    auto feed = m_feedsModel.getFeed(url).lock();
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
