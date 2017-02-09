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
    m_view{std::move(view)}
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
    getFeedsFromConfig();
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

void Application::getFeedsFromConfig()
{
    const auto progQtPath = std::vector<QString>{ "programming", "qt" };
    const auto graphicsPath = std::vector<QString>{ "graphics" };

    bool success;

    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Folder>(progQtPath[0]), std::vector<QString>{});
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Folder>(progQtPath[1]), std::vector<QString>{progQtPath[0]});
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Folder>(graphicsPath[0]), std::vector<QString>{});
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("Qt Blog", "The official Qt Blog", QUrl("http://blog.qt.io/feed")), progQtPath);
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("KDAB Blogs", "KDAB Blogs", QUrl("https://www.kdab.com/category/blogs/feed/")), progQtPath);
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("Pushing Pixels", "Kirill Grouchnikovs Blog", QUrl("http://www.pushing-pixels.org/feed")), graphicsPath);
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("The ryg blog", "asdf", QUrl("https://fgiesen.wordpress.com/feed/")), std::vector<QString>{progQtPath[0]});
    Q_ASSERT(success);
}

}  // namespace feedling
