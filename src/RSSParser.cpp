#include "RSSParser.hpp"

#include <sstream>

#include <QtCore/QtDebug>
#include <QtCore/QCryptographicHash>

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
    QByteArray id;
    QDateTime date;

    while (m_xmlReader.readNextStartElement()) {
        auto name = m_xmlReader.name();

        if (name == "title") {
            title = m_xmlReader.readElementText();
        } else if (name == "description") {
            content = m_xmlReader.readElementText();
        } else if (name == "guid") {
            id = m_xmlReader.readElementText().toUtf8();
        } else if (name == "pubDate") {
            date = QDateTime::fromString(m_xmlReader.readElementText());
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }

    if (id.isEmpty()) {
        // The <guid> element is optional in RSS, so we have to create an ID ourselves.
        // Unfortunately, the specification is extremely loose on the requirements. Everything is
        // optional except for a title or a description, but we can't know which one we have.
        // Therefore, we hash the content if there is one, else we hash the title.

        id = QCryptographicHash::hash(content.isEmpty() ? title.toUtf8() : content.toUtf8(),
                                      QCryptographicHash::Md5)
                .toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
    }

    auto feed = FeedParser::feed();
    if (!feed->getEntry(id)) {
        feed->addEntry(std::make_shared<Entry>(title, content, id, date, feed));
    }
    // TODO: check if entry was updated
}

}  // namespace feedling
