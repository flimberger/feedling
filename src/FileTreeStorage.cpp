#include "FileTreeStorage.hpp"

#include <algorithm>

#include <QtCore/QtDebug>

#include "Feed.hpp"

namespace feedling {

FileTreeStorage::FileTreeStorage(const QDir &storageRoot)
  : IStorage(),
    m_storageRoot{storageRoot}
{}

FileTreeStorage::~FileTreeStorage()
{}

bool FileTreeStorage::init()
{
    return true;
}

std::vector<std::shared_ptr<Feed>> FileTreeStorage::readFeeds()
{
    auto feeds = std::vector<std::shared_ptr<Feed>>{};
    const auto folders = m_storageRoot.entryList();
    std::for_each(std::cbegin(folders), std::cend(folders), [](const QString &name) {
        qDebug() << name;
    });
    return feeds;
}

bool FileTreeStorage::writeFeed(const std::shared_ptr<Feed> &feed)
{
    return false;
}

}  // namespace feedling
