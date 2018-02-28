#ifndef FEEDLING_FOLDER_HPP
#define FEEDLING_FOLDER_HPP

#include <memory>
#include <vector>

#include "TreeItem.hpp"

namespace feedling {

class Folder : public TreeItem
{
public:
    Folder(QString name);
    virtual ~Folder();

    const std::vector<std::unique_ptr<TreeItem>> &items() const;
    void addItem(std::unique_ptr<TreeItem> item);

    int size() const;
    const TreeItem *getItem(QString name) const;
    const TreeItem *getItem(int idx) const;
    TreeItem *getItem(int idx);

private:
    std::vector<std::unique_ptr<TreeItem>> m_items;
};

}  // namespace feedling

#endif  // FEEDLING_FOLDER_HPP
