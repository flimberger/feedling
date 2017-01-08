#ifndef FEEDLING_VIEW_HPP
#define FEEDLING_VIEW_HPP

class QAbstractItemModel;

namespace feedling {

class View {
public:
    virtual ~View();

    virtual void init() = 0;
    virtual void setFeedsModel(QAbstractItemModel *model) = 0;
    virtual void setEntriesModel(QAbstractItemModel *model) = 0;
};

}  // namespace feedling

#endif  // FEEDLING_VIEW_HPP
