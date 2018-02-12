#ifndef FEEDLING_FEEDPARSER_HPP
#define FEEDLING_FEEDPARSER_HPP

#include <memory>
#include <string_view>

namespace feedling {

class Feed;

class FeedParser
{
public:
    // feed: feed to which the parsed data is added
    // data: raw XML data of the feed
    static bool parseXml(const std::shared_ptr<Feed> &feed, std::string_view data);
};

}  // namespace feedling

#endif  // FEEDLING_FEEDPARSER_HPP
