#include "RSSParser.hpp"

#include <sstream>

#include <QtCore/QtDebug>

#include "Entry.hpp"
#include "Feed.hpp"

namespace feedling {

RSSParser::RSSParser(const std::shared_ptr<Feed> &feed) : FeedParser{feed}
{}

bool RSSParser::read(QIODevice *ioDevice)
{
    m_xmlReader.setDevice(ioDevice);

    if (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == "rss") {
            while (m_xmlReader.readNextStartElement()) {
                if (m_xmlReader.name() == "channel") {
                    readChannel();
                } else {
                    std::stringstream s{};
                    s << "Encountered \"" << m_xmlReader.name().toString().toUtf8().data()
                      << "\" instead of \"channel\"";
                    m_xmlReader.raiseError(QString::fromUtf8(s.str().c_str()));
                }
            }
        } else {
            m_xmlReader.raiseError("This is not a RSS file");
        }
    }

    return !m_xmlReader.error();
}

QString RSSParser::errorString() const
{
    return m_xmlReader.errorString();
}

void RSSParser::readChannel()
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "channel");

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == "item") {
            readItem();
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void RSSParser::readItem()
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "item");

    QString title;
    QString content;
    QString id;
    QDateTime date;

    while (m_xmlReader.readNextStartElement()) {
        auto name = m_xmlReader.name();

        if (name == "title") {
            title = m_xmlReader.readElementText();
        } else if (name == "description") {
            content = m_xmlReader.readElementText();
        } else if (name == "guid") {
            id = m_xmlReader.readElementText();
        } else if (name == "pubDate") {
            date = QDateTime::fromString(m_xmlReader.readElementText());
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }

    feed()->addEntry(std::make_shared<Entry>(title, content, id, date, feed()));
}

}  // namespace feedling
