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
    const auto progQtPath = std::vector<QString>{ "programming", "qt" };
    const auto graphicsPath = std::vector<QString>{ "graphics" };

    bool success;

    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Folder>(progQtPath[0]), std::vector<QString>{});
    Q_ASSERT(success);
    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Folder>(progQtPath[1]), std::vector<QString>{progQtPath[0]});
    Q_ASSERT(success);
    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Folder>(graphicsPath[0]), std::vector<QString>{});
    Q_ASSERT(success);
    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Feed>("Qt Blog", "The official Qt Blog", QUrl("http://blog.qt.io/feed")), progQtPath);
    Q_ASSERT(success);
    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Feed>("KDAB Blogs", "KDAB Blogs", QUrl("https://www.kdab.com/category/blogs/feed/")), progQtPath);
    Q_ASSERT(success);
    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Feed>("Pushing Pixels", "Kirill Grouchnikovs Blog", QUrl("http://www.pushing-pixels.org/feed")), graphicsPath);
    Q_ASSERT(success);
    success = m_feedsModel->addItem<std::vector<QString>>(std::make_shared<Feed>("The ryg blog", "asdf", QUrl("https://fgiesen.wordpress.com/feed/")), std::vector<QString>{progQtPath[0]});
    Q_ASSERT(success);
}

}  // namespace feedling
