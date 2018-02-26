#include "AtomParser.hpp"

#include <QtCore/QtDebug>

#include "Entry.hpp"
#include "Feed.hpp"

namespace feedling {

Text::Text() : type{Plain}, text{} {}

Text::Text(Type type, QString text) : type{type}, text{std::move(text)} {}

Person::Person() = default;

Person::Person(Text name, QUrl uri, QString email)
    : name{std::move(name)},
      uri{std::move(uri)},
      email{std::move(email)}
{}

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

QDateTime AtomParser::readDateConstruct() {
    Q_ASSERT(m_xmlReader.isStartElement());

    auto text = m_xmlReader.readElementText();
    auto date = QDateTime::fromString(text, Qt::ISODate);

    if (!date.isValid()) {
        qWarning() << "invalid date:" << text;
    }

    return date;
}

void AtomParser::readEntry() {
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "entry");

    Person author;  // currently ignored
    Text title;  // the text type is currently ignored
    Text content;
    QByteArray id;
    QDateTime date;

    while (m_xmlReader.readNextStartElement()) {
        auto name = m_xmlReader.name();
        qDebug() << "parsing element" << name;
        if (name == "author") {
            author = readPersonConstruct();
        } if (name == "id") {
            id = m_xmlReader.readElementText().toUtf8();
        } else if (name == "title") {
            title = readTextConstruct();
        } else if (name == "published") {
            if (!date.isValid()) {  // only read the date if this entry was not updated
                date = readDateConstruct();
            }
        } else if (name == "updated") {
            date = readDateConstruct();
        } else if (name == "content" || name == "summary") {
            content = readTextConstruct();
        } else {
            qDebug() << "unknown element" << name;
            m_xmlReader.skipCurrentElement();
        }
    }

    qDebug() << "parsed entry" << title.text;
    auto feed = FeedParser::feed();
    if (!feed->getEntry(id)) {
        feed->addEntry(std::make_shared<Entry>(title.text, content.text, id, date, feed));
    }
    // TODO: check if entry was updated
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

Person AtomParser::readPersonConstruct() {
    Q_ASSERT(m_xmlReader.isStartElement());

    Text name;
    QString uri;
    QString email;

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == "name") {
            name = readTextConstruct();
        } else if (m_xmlReader.name() == "uri") {
            uri = m_xmlReader.readElementText();
        } else if (m_xmlReader.name() == "email") {
            email = m_xmlReader.readElementText();
        }
    }

    return Person{name, uri, email};
}

Text AtomParser::readTextConstruct() {
    Q_ASSERT(m_xmlReader.isStartElement());

    Text::Type type = Text::Plain;

    for (const auto &attribute : m_xmlReader.attributes()) {
        if (attribute.name() == "type") {
            if (attribute.value() == "html") {
                type = Text::HTML;
            } else if (attribute.value() == "xhtml") {
                type = Text::XHTML;
            } else if (attribute.value() != "text") {
                qWarning() << "ignoring invalid text type" << attribute.value();
            }
        }
    }

    return Text{type, m_xmlReader.readElementText()};
}

}  // namespace feedling
