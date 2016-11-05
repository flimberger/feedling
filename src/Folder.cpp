#include "Folder.hpp"

#include <climits>

namespace feedling {

Folder::Folder(QString name)
  : FeedsModelItem(name, FeedsModelItem::Type::FOLDER)
{}

Folder::~Folder() = default;

const std::vector<std::shared_ptr<FeedsModelItem>> &Folder::items() const
{
    return m_items;
}

void Folder::addItem(const std::shared_ptr<FeedsModelItem> &item)
{
    m_items.emplace_back(item);
    // item->setFolder(nullptr);// shared_from_this());
}

int Folder::size() const
{
    Q_ASSERT(m_items.size() < INT_MAX);
    return static_cast<int>(m_items.size());
}

std::shared_ptr<FeedsModelItem> Folder::getItem(QString name) const
{
    const auto &end = std::end(m_items);
    const auto &item = std::find_if(std::begin(m_items), end,
                                    [name](const std::shared_ptr<FeedsModelItem> &item) {
        return item->name() == name;
    });
    if (item != end) {
        return *item;
    }
    return nullptr;
}

}  // namespace feedling
