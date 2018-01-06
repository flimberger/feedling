#ifndef FEEDLING_TEXTFILEDATASTORE_H
#define FEEDLING_TEXTFILEDATASTORE_H

#include <QtCore/QString>

namespace feedling {

class FeedsModel;

// An implementation of the datastore which stores the feeds in plain text files in a directory
// hierarchy.
class TextFileDataStore
{
public:
    TextFileDataStore(const QString &baseDirectory, FeedsModel *model);

    // TODO: this should be asynchronous
    void fetchFeeds();

private:
    QString m_baseDirectory;
    FeedsModel *m_feedsModel;
};

}  // namespace feedling

#endif  // FEEDLING_TEXTFILEDATASTORE_H
