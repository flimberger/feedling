#ifndef FEEDLING_FOLDER_HPP
#define FEEDLING_FOLDER_HPP

#include <memory>
#include <vector>

#include "FeedsModelItem.hpp"

namespace feedling {

class Folder : public FeedsModelItem //, public std::enable_shared_from_this<Folder>
{
public:
    Folder(QString name);
    virtual ~Folder();

    const std::vector<std::shared_ptr<FeedsModelItem>> &items() const;
    void addItem(const std::shared_ptr<FeedsModelItem> &item);

    int size() const;
    std::shared_ptr<FeedsModelItem> getItem(QString name) const;

private:
    std::vector<std::shared_ptr<FeedsModelItem>> m_items;
};

}  // namespace feedling

#endif  // FEEDLING_FOLDER_HPP
