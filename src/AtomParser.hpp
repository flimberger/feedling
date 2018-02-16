#ifndef FEEDLING_ATOM_PARSER_HPP
#define FEEDLING_ATOM_PARSER_HPP

#include <memory>

#include <QtCore/QXmlStreamReader>

#include "FeedParser.hpp"

namespace feedling {

// Simple parser which currently only understands Atom
class AtomParser : public FeedParser
{
public:
    AtomParser(const std::shared_ptr<Feed> &m_feed);
    ~AtomParser();

    bool read(QIODevice *ioDevice) override;

    QString errorString() const override;

private:
    void readEntry();
    void readFeed();

    QXmlStreamReader m_xmlReader;
};

}  // namespace feedling

#endif  // FEEDLING_ATOM_PARSER_HPP
