#ifndef FEEDLING_PRESENTER_HPP
#define FEEDLING_PRESENTER_HPP

#include <memory>

class QModelIndex;

namespace feedling {

class Entry;
class Feed;

class Presenter
{
public:
    virtual ~Presenter();

    virtual void selectFeed(const QModelIndex &index) = 0;
    virtual void selectEntry(const QModelIndex &index) = 0;
};

}  // namespace feedling

#endif  // FEEDLING_PRESENTER_HPP
