#ifndef FEEDLING_FILETREESTORAGE_HPP
#define FEEDLING_FILETREESTORAGE_HPP

#include "IStorage.hpp"

#include <QtCore/QDir>

namespace feedling {

class FileTreeStorage : public IStorage
{
public:
    FileTreeStorage(const QDir &storageRoot);
    ~FileTreeStorage();

    bool init() override;
    std::vector<std::shared_ptr<Feed>> readFeeds() override;
    bool writeFeed(const std::shared_ptr<Feed> &feed) override;

private:
    QDir m_storageRoot;
};

}  // namespace feedling

#endif  // FEEDLING_FILETREESTORAGE_HPP
