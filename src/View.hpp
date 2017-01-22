#ifndef FEEDLING_VIEW_HPP
#define FEEDLING_VIEW_HPP

#include <memory>

class QAbstractItemModel;

namespace feedling {

class Entry;
class Presenter;

class View {
public:
    virtual ~View();

    virtual void init(Presenter *presenter) = 0;
    virtual void setFeedsModel(QAbstractItemModel *model) = 0;
    virtual void setEntriesModel(QAbstractItemModel *model) = 0;
    virtual void showEntry(const std::shared_ptr<Entry> &entry) = 0;
};

}  // namespace feedling

#endif  // FEEDLING_VIEW_HPP
