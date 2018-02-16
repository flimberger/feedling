#include "FeedParser.hpp"

namespace feedling {

FeedParser::FeedParser(const std::shared_ptr<Feed> &feed) : m_feed{feed} {}

FeedParser::~FeedParser() = default;

}  // namespace feedling
