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

FeedParser::FeedParser(std::shared_ptr<Feed> feed, QIODevice *ioDevice, QObject *parent)
  : QObject{parent},
    m_xmlReader{std::make_unique<QXmlStreamReader>()},
    m_feed{feed},
    m_ioDevice{ioDevice}
{
    QObject::connect(m_ioDevice, &QIODevice::readyRead, this, &FeedParser::onDataReady);
}

FeedParser::~FeedParser() = default;

void FeedParser::parseXml() {
    QString content;
    QString descr;
    QString link;
    QDateTime pubDate;
    QString title;

    auto url = m_feed->url().toString();
    qDebug() << "Starting to parse " << url;
    while (!m_xmlReader->atEnd()) {
        qDebug() << "parser loop";
        m_xmlReader->readNext();
        if (m_xmlReader->isStartElement()) {
            m_currentTag = m_xmlReader->name().toString();
        } else if (m_xmlReader->isEndElement()) {
            if (m_xmlReader->name() == TAGNAME_ITEM) {
                // create entry
                m_feed->addEntry(std::make_shared<Entry>(title, content, pubDate, m_feed));
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
            } else if (m_currentTag == TAGNAME_TITLE) {
                title = m_xmlReader->text().toString();
            }
        }
    }
    if (m_xmlReader->hasError()) {
        if (m_xmlReader->error() == QXmlStreamReader::PrematureEndOfDocumentError) {
            qDebug() << "Waiting for more data from " << url;
        } else {
            qDebug() << "Parsing of " << url << " failed";
            emit done(false, m_feed);
        }
    } else {
        qDebug() << "Parsing of " << url << " finished";
        emit done(true, m_feed);
    }
}

void FeedParser::onDataReady() {
    auto data = m_ioDevice->readAll();
    qDebug() << "BEDIN DATA";
    qDebug() << data;
    qDebug() << "END DATA";
    m_xmlReader->addData(data);
    parseXml();
}

}  // namespace feedling
