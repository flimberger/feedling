#ifndef FEEDLING_FOLDER_HPP
#define FEEDLING_FOLDER_HPP

#include <memory>
#include <vector>

#include "TreeItem.hpp"

namespace feedling {

class Folder : public TreeItem
{
public:
    using value_type = std::unique_ptr<TreeItem>;
    using container_type = std::vector<value_type>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;

    Folder(QString name);
    virtual ~Folder();

    void addItem(std::unique_ptr<TreeItem> item);

    int size() const;
    const TreeItem *getItem(QString name) const;
    const TreeItem *getItem(int idx) const;
    TreeItem *getItem(int idx);

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

private:
    container_type m_items;
};

}  // namespace feedling

#endif  // FEEDLING_FOLDER_HPP
