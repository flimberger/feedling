#include "FeedParser.hpp"

#include <QtCore/QtDebug>
#include <QtCore/QDateTime>
#include <QtCore/QXmlStreamReader>

#include "Entry.hpp"
#include "Feed.hpp"

namespace feedling {

static constexpr const char *TAGNAME_CONTENT = "encoded";  // content:encoded
static constexpr const char *TAGNAME_DESCR = "description";
static constexpr const char *TAGNAME_ITEM = "item";
static constexpr const char *TAGNAME_LINK = "link";
static constexpr const char *TAGNAME_PUBDATE = "pubDate";
static constexpr const char *TAGNAME_TITLE = "title";

bool FeedParser::parseXml(const std::shared_ptr<Feed> &feed, std::string_view data) {
    QXmlStreamReader xmlReader;
    QString content;
    QString currentTag;
    QString descr;
    QString link;
    QDateTime pubDate;
    QString title;

    xmlReader.addData(QByteArray{data.data()});
    auto url = feed->url().toString();
    qDebug() << "Starting to parse " << url;
    while (!xmlReader.atEnd()) {
        qDebug() << "parser loop";
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            currentTag = xmlReader.name().toString();
        } else if (xmlReader.isEndElement()) {
            if (xmlReader.name() == TAGNAME_ITEM) {
                // create entry
                feed->addEntry(std::make_shared<Entry>(title, content, pubDate, feed));
                // Clear strings
                content.clear();
                descr.clear();
                link.clear();
                // pubDate.clear();
                title.clear();
            }
        } else if ((xmlReader.isCharacters() || xmlReader.isCDATA())
                   && !xmlReader.isWhitespace()) {
            if (currentTag == TAGNAME_CONTENT) {
                content = xmlReader.text().toString();
            } else if (currentTag == TAGNAME_DESCR) {
                descr = TAGNAME_DESCR;
            } else if (currentTag == TAGNAME_LINK) {
                link = xmlReader.text().toString();
            } else if (currentTag == TAGNAME_PUBDATE) {
                pubDate = QDateTime::fromString(xmlReader.text().toString());
            } else if (currentTag == TAGNAME_TITLE) {
                title = xmlReader.text().toString();
            }
        }
    }
    if (xmlReader.hasError()) {
        qDebug() << "Parsing of " << url << " failed";

        return false;
    } else {
        qDebug() << "Parsing of " << url << " finished";

        return true;
    }
}

}  // namespace feedling
