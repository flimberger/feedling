#ifndef FEEDLING_ATOM_PARSER_HPP
#define FEEDLING_ATOM_PARSER_HPP

#include <memory>

#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>

#include "FeedParser.hpp"

namespace feedling {

struct Text {
    enum Type {
        Plain,
        HTML,
        XHTML
    } type;  // default value: Plain
    QString text;  // required

    Text();
    Text(Type type, QString text);
};

struct Person {
    Text name;  // required
    QUrl uri;  // optional
    QString email;  // optional

    Person();
    Person(Text name, QUrl uri, QString email);
};

// Simple parser which currently only understands Atom
class AtomParser : public FeedParser
{
public:
    AtomParser(const std::shared_ptr<Feed> &m_feed);
    ~AtomParser();

    bool read(QIODevice *ioDevice) override;

    QString errorString() const override;

private:
    QDateTime readDateConstruct();
    void readEntry();
    void readFeed();
    Person readPersonConstruct();
    Text readTextConstruct();

    QXmlStreamReader m_xmlReader;
};

}  // namespace feedling

#endif  // FEEDLING_ATOM_PARSER_HPP
