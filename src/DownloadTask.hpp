#ifndef FEEDLING_DOWNLOADTASK_HPP
#define FEEDLING_DOWNLOADTASK_HPP

#include <memory>

#include <QtCore/QObject>

#include "FeedParser.hpp"

class QNetworkReply;

namespace feedling {

class Feed;

class DownloadTask : public QObject
{
    Q_OBJECT
public:
    DownloadTask(const std::shared_ptr<Feed> &feed, QNetworkReply *reply, QObject *parent=nullptr);
    ~DownloadTask();

    Q_SIGNAL void done(bool success, const std::shared_ptr<Feed> &feed);

private:
    Q_SLOT void onReadyRead();

    FeedParser m_feedParser;
    QNetworkReply *m_reply;
};

}  // namespace feedling

#endif  // FEEDLING_DOWNLOADTASK_HPP
