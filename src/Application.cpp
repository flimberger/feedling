#include "Application.hpp"

#include <iterator>
#include <vector>

#include <QtCore/QtDebug>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtQml/QQmlContext>

#include "Feed.hpp"

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

    m_qmlAppEngine.rootContext()->setContextProperty("feedsModel", &m_feedsModel);
    m_qmlAppEngine.load(QUrl("qrc:///ui/main.qml"));
}

void Application::onFetchFeeds()
{
//    for (const auto &feed : m_feedsModel.feeds()) {
//        m_network->get(QNetworkRequest(feed->url()));
//    }
}

void Application::onFeedDownloadFinished(QNetworkReply *reply)
{
    const auto &url = reply->url();
    auto feed = m_feedsModel.getFeed(url).lock();
    if (feed) {
        auto data = reply->readAll();
        qDebug() << data;
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
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("Qt Blog", "The official Qt Blog", QUrl("http://blog.qt.io/feed")), progQtPath);
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("KDAB Blogs", "KDAB Blogs", QUrl("https://www.kdab.com/category/blogs/feed/")), progQtPath);
    Q_ASSERT(success);
    success = m_feedsModel.addItem<std::vector<QString>>(std::make_shared<Feed>("Pushing Pixels", "Kirill Grouchnikovs Blog", QUrl("http://www.pushing-pixels.org/feed")), graphicsPath);
    Q_ASSERT(success);
}

}  // namespace feedling
