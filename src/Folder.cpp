#include "Folder.hpp"

#include <climits>

namespace feedling {

Folder::Folder(QString name)
  : TreeItem(name, TreeItem::Type::FOLDER)
{}

Folder::~Folder() = default;

void Folder::addItem(std::unique_ptr<TreeItem> item)
{
    item->setFolder(this);
    m_items.emplace_back(std::move(item));
}

int Folder::size() const
{
    Q_ASSERT(m_items.size() < INT_MAX);
    return static_cast<int>(m_items.size());
}

const TreeItem *Folder::getItem(QString name) const
{
    const auto &end = std::end(m_items);
    const auto &item = std::find_if(std::begin(m_items), end,
                                    [name](const std::unique_ptr<TreeItem> &item) {
        return item->name() == name;
    });
    if (item != end) {
        return item->get();
    }
    return nullptr;
}

const TreeItem *Folder::getItem(int idx) const
{
    Q_ASSERT(idx >= 0);

    return m_items[static_cast<decltype(m_items)::size_type>(idx)].get();
}

TreeItem *Folder::getItem(int idx)
{
    return const_cast<TreeItem *>(static_cast<const Folder *>(this)->getItem(idx));
}

Folder::iterator Folder::begin() { return std::begin(m_items); }
Folder::const_iterator Folder::begin() const { return std::cbegin(m_items); }
Folder::const_iterator Folder::cbegin() const { return std::cbegin(m_items); }

Folder::iterator Folder::end() { return std::end(m_items); }
Folder::const_iterator Folder::end() const { return std::cend(m_items); }
Folder::const_iterator Folder::cend() const { return std::cend(m_items); }

}  // namespace feedling
