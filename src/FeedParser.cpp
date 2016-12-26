#include "FeedParser.hpp"

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

FeedParser::FeedParser(std::shared_ptr<Feed> feed, QIODevice *ioDevice)
  : m_xmlReader{std::make_unique<QXmlStreamReader>(ioDevice)},
    m_feed{feed}
{}

FeedParser::~FeedParser() = default;

void FeedParser::parseXml() {
    QString content;
    QString descr;
    QString link;
    QDateTime pubDate;
    QString title;

    while (!m_xmlReader->atEnd()) {
        m_xmlReader->readNext();
        if (m_xmlReader->isStartElement()) {
            m_currentTag = m_xmlReader->name().toString();
        } else if (m_xmlReader->isEndElement()) {
            if (m_xmlReader->name() == TAGNAME_ITEM) {
                // create entry
                m_feed->addEntry(Entry{title, content, pubDate, m_feed});
                // Clear strings
                content.clear();
                descr.clear();
                link.clear();
                // pubDate.clear();
                title.clear();
            }
        } else if ((m_xmlReader->isCharacters() || m_xmlReader->isCDATA())
                   && !m_xmlReader->isWhitespace()) {
            if (m_currentTag == TAGNAME_CONTENT) {
                content = m_xmlReader->text().toString();
            } else if (m_currentTag == TAGNAME_DESCR) {
                descr = TAGNAME_DESCR;
            } else if (m_currentTag == TAGNAME_LINK) {
                link = m_xmlReader->text().toString();
            } else if (m_currentTag == TAGNAME_PUBDATE) {
                pubDate = QDateTime::fromString(m_xmlReader->text().toString());
            } if (m_currentTag == TAGNAME_TITLE) {
                title = m_xmlReader->text().toString();
            }
        }
    }
}

}  // namespace feedling
