#include "FeedParser.hpp"

#include <QtCore/QtDebug>
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

FeedParser::FeedParser(std::shared_ptr<Feed> feed)
  : m_xmlReader{std::make_unique<QXmlStreamReader>()},
    m_feed{feed},
    m_state{STATE_WAITING}
{
}

FeedParser::~FeedParser() {
    qDebug() << "Parser for " << m_feed->url().toString() << " died";
}

FeedParser::State FeedParser::parse() {
    auto url = m_feed->url().toString();
    qDebug() << "Starting to parse " << url;
    while (!m_xmlReader->atEnd()) {
        m_xmlReader->readNext();
        if (m_xmlReader->isStartElement()) {
            m_currentTag = m_xmlReader->name().toString();
        } else if (m_xmlReader->isEndElement()) {
            if (m_xmlReader->name() == TAGNAME_ITEM) {
                // create entry
                m_feed->addEntry(std::make_shared<Entry>(m_title, m_content, m_pubDate, m_feed));
                // Clear strings
                m_content.clear();
                m_descr.clear();
                m_link.clear();
                // m_pubDate.clear();
                m_title.clear();
            }
        } else if ((m_xmlReader->isCharacters() || m_xmlReader->isCDATA())
                   && !m_xmlReader->isWhitespace()) {
            if (m_currentTag == TAGNAME_CONTENT) {
                m_content = m_xmlReader->text().toString();
            } else if (m_currentTag == TAGNAME_DESCR) {
                m_descr = TAGNAME_DESCR;
            } else if (m_currentTag == TAGNAME_LINK) {
                m_link = m_xmlReader->text().toString();
            } else if (m_currentTag == TAGNAME_PUBDATE) {
                m_pubDate = QDateTime::fromString(m_xmlReader->text().toString());
            } else if (m_currentTag == TAGNAME_TITLE) {
                m_title = m_xmlReader->text().toString();
            } else {
                qDebug() << "unknown tag " << m_currentTag;
            }
        }
    }
    if (m_xmlReader->hasError()) {
        if (m_xmlReader->error() == QXmlStreamReader::PrematureEndOfDocumentError) {
            qDebug() << "Waiting for more data from " << url;
            m_state = STATE_WAITING;
        } else {
            qDebug() << "Parsing of " << url << " failed:" << m_xmlReader->errorString();
            m_state = STATE_FAILED;
        }
    } else {
        qDebug() << "Parsing of " << url << " finished";
        m_state = STATE_SUCCEEDED;
    }
    return m_state;
}

void FeedParser::addData(const QByteArray &data)
{
    m_xmlReader->addData(data);
}

FeedParser::State FeedParser::state() const
{
    return m_state;
}


}  // namespace feedling
