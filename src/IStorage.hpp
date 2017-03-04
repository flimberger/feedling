#ifndef FEEDLING_STORAGE_HPP
#define FEEDLING_STORAGE_HPP

#include <memory>
#include <vector>

namespace feedling {

class Feed;

class IStorage
{
public:
    virtual ~IStorage();

    virtual bool init() = 0;
    // TODO: use iterator or the like
    virtual std::vector<std::shared_ptr<Feed>> readFeeds() = 0;
    virtual bool writeFeed(const std::shared_ptr<Feed> &feed) = 0;
};

}  // namespace feedling

#endif  // FEEDLING_STORAGE_HPP
