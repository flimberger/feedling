#ifndef FEEDLING_RSSPARSER_HPP
#define FEEDLING_RSSPARSER_HPP

#include <QtCore/QXmlStreamReader>

#include "FeedParser.hpp"

namespace feedling {

class RSSParser : public FeedParser
{
public:
    RSSParser(const std::shared_ptr<Feed> &feed);

    bool read(QIODevice *ioDevice) override;
    QString errorString() const override;

private:
    void readChannel();
    void readItem();

    QXmlStreamReader m_xmlReader;
};

}  // namespace feedling

#endif  // FEEDLING_RSSPARSER_HPP
