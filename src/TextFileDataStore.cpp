#include "TextFileDataStore.hpp"

#include "Feed.hpp"
#include "FeedsModel.hpp"

namespace feedling {

TextFileDataStore::TextFileDataStore(const QString &baseDirectory, FeedsModel *model)
  : m_baseDirectory{baseDirectory},
    m_feedsModel{model}
{
    // TODO: watch feeds for changes
}

void TextFileDataStore::fetchFeeds()
{
    auto *progFolder = m_feedsModel->addFolder("programming", nullptr);
    auto *qtFolder = m_feedsModel->addFolder("qt", progFolder);
    auto *gfxFolder = m_feedsModel->addFolder("graphics", nullptr);

    m_feedsModel->addFeed(std::make_shared<Feed>("Qt Blog", "The official Qt Blog", QUrl("http://blog.qt.io/feed")), qtFolder);
    m_feedsModel->addFeed(std::make_shared<Feed>("KDAB Blogs", "KDAB Blogs", QUrl("https://www.kdab.com/category/blogs/feed/")), qtFolder);
    m_feedsModel->addFeed(std::make_shared<Feed>("Pushing Pixels", "Kirill Grouchnikovs Blog", QUrl("http://www.pushing-pixels.org/feed")), gfxFolder);
    m_feedsModel->addFeed(std::make_shared<Feed>("The ryg blog", "asdf", QUrl("https://fgiesen.wordpress.com/feed/")), progFolder);
}

}  // namespace feedling
