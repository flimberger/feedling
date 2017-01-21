#ifndef FEEDLING_FOLDER_HPP
#define FEEDLING_FOLDER_HPP

#include <memory>
#include <vector>

#include "TreeItem.hpp"

namespace feedling {

class Folder : public TreeItem, public std::enable_shared_from_this<Folder>
{
public:
    Folder(QString name);
    virtual ~Folder();

    const std::vector<std::shared_ptr<TreeItem>> &items() const;
    void addItem(const std::shared_ptr<TreeItem> &item);

    int size() const;
    std::shared_ptr<TreeItem> getItem(QString name) const;
    std::shared_ptr<TreeItem> getItem(int idx) const;

private:
    std::vector<std::shared_ptr<TreeItem>> m_items;
};

}  // namespace feedling

#endif  // FEEDLING_FOLDER_HPP
