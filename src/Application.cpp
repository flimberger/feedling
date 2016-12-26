#include "Application.hpp"

#include <iterator>
#include <vector>

#include <QtCore/QtDebug>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtQml/QQmlContext>

#include "EntriesModel.hpp"
#include "Feed.hpp"
#include "FeedParser.hpp"

namespace feedling {

Application::Application(QObject *parent)
  : QObject(parent),
    m_network(new QNetworkAccessManager(this))
{
    connect(m_network, &QNetworkAccessManager::finished,
            this,      &Application::onFeedDownloadFinished);
    onCreated();
}

Application::~Application() = default;

void Application::onCreated()
{
    // TODO: make this asynchronous
    getFeedsFromConfig();

    m_qmlAppEngine.rootContext()->setContextProperty("g_app", this);
    m_qmlAppEngine.rootContext()->setContextProperty("feedsModel", &m_feedsModel);
    m_qmlAppEngine.load(QUrl("qrc:///ui/main.qml"));
}

void Application::onFetchFeeds()
{
    for (const auto &wref : m_feedsModel.feeds()) {
        if (const auto feed = wref.lock()) {
            m_network->get(QNetworkRequest(feed->url()));
        }
    }
}

void Application::setEntryList(QUrl url)
{
    auto ptr = m_feedsModel.getFeed(url).lock();
    if (ptr) {
        m_entriesModel = std::make_unique<EntriesModel>(ptr);
        m_qmlAppEngine.rootContext()->setContextProperty("g_entriesModel", m_entriesModel.get());
    }
}

void Application::onFeedDownloadFinished(QNetworkReply *reply)
{
    const auto &url = reply->url();
    auto feed = m_feedsModel.getFeed(url).lock();
    if (feed) {
        qDebug() << "Parsing " << url;
        FeedParser parser{feed, reply};
        parser.parseXml();
        for (const auto &e : feed->entries()) {
            qDebug() << e.title() << e.dateTime().toString() << e.content();
        }
    } else {
        qWarning() << "unknown URL:" << url;
    }

    reply->deleteLater();
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
//    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("Qt Blog", "The official Qt Blog", QUrl("http://blog.qt.io/feed")), progQtPath);
//    Q_ASSERT(success);
//    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("KDAB Blogs", "KDAB Blogs", QUrl("https://www.kdab.com/category/blogs/feed/")), progQtPath);
//    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("Pushing Pixels", "Kirill Grouchnikovs Blog", QUrl("http://www.pushing-pixels.org/feed")), graphicsPath);
    Q_ASSERT(success);

    setEntryList(QUrl("http://blog.qt.io/feed"));
}

}  // namespace feedling
