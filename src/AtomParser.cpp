#include "AtomParser.hpp"

#include <QtCore/QtDebug>

#include "Entry.hpp"
#include "Feed.hpp"

namespace feedling {

AtomParser::AtomParser(const std::shared_ptr<Feed> &feed) : FeedParser{feed} {}

AtomParser::~AtomParser() = default;

bool AtomParser::read(QIODevice *ioDevice) {
    m_xmlReader.setDevice(ioDevice);

    if (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == "feed") {
            readFeed();
        } else if (m_xmlReader.name() == "entry") {
            readEntry();
        } else {
            m_xmlReader.raiseError("This is not an Atom file");
        }
    }

    return !m_xmlReader.error();
}

QString AtomParser::errorString() const {
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_xmlReader.errorString())
            .arg(m_xmlReader.lineNumber())
            .arg(m_xmlReader.columnNumber());
}

void AtomParser::readEntry() {
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "entry");

    QString title;
    QString content;
    QDateTime date;

    while (m_xmlReader.readNextStartElement()) {
        qDebug() << "parsing element" << m_xmlReader.name();
        if (m_xmlReader.name() == "title") {
            title = m_xmlReader.readElementText();
        } else if (m_xmlReader.name() == "content") {
            content = m_xmlReader.readElementText();
        } else if (m_xmlReader.name() == "published") {
            date = QDateTime::fromString(m_xmlReader.readElementText());
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }

    qDebug() << "parsed entry" << title;
    auto feed = FeedParser::feed();
    feed->addEntry(std::make_shared<Entry>(title, content, date, feed));
}

void AtomParser::readFeed() {
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "feed");

    while (m_xmlReader.readNextStartElement()) {
        // for now only entries are consumed
        if (m_xmlReader.name() == "entry") {
            readEntry();
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

}  // namespace feedling
