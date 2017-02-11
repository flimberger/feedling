#include "DownloadTask.hpp"

#include <QtNetwork/QNetworkReply>

namespace feedling {

DownloadTask::DownloadTask(const std::shared_ptr<Feed> &feed, QNetworkReply *reply, QObject *parent)
  : QObject(parent),
    m_feedParser{feed},
    m_reply{reply}
{
    QObject::connect(m_reply, &QNetworkReply::readyRead, this, &DownloadTask::onReadyRead);
}

DownloadTask::~DownloadTask()
{
    // TODO: a custom ``QtLaterDeleter`` with a std::unique_ptr would be handy
    m_reply->deleteLater();
}

void DownloadTask::onReadyRead()
{
    m_feedParser.addData(m_reply->readAll());
    const auto state = m_feedParser.parse();
    switch (state) {
    case FeedParser::STATE_WAITING:
        // do nothing;
        return;
    case FeedParser::STATE_SUCCEEDED:
        emit done(true, m_feedParser.feed());
        break;
    case FeedParser::STATE_FAILED:
        // signal failure
        emit done(false, m_feedParser.feed());
        break;
    }
}

}  // namespace feedling
