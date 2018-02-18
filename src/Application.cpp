#include "Application.hpp"

#include <iterator>
#include <vector>
#include <utility>

#include <QtCore/QtDebug>

#include <QtQml/QQmlContext>

#include "EntriesModel.hpp"
#include "Feed.hpp"
#include "View.hpp"

namespace feedling {

Application::Application(std::unique_ptr<View> &&view, QObject *parent)
  : QObject{parent},
    Presenter{},
    m_view{std::move(view)},
    m_fetcher{&m_feedsModel},
    m_dataStore{".", &m_feedsModel}
{
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
    m_fetcher.onFetch();
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

}  // namespace feedling
