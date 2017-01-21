#include "TreeItem.hpp"

#include "Folder.hpp"

namespace feedling {

TreeItem::TreeItem(QString name, Type type)
  : m_name(name),
    m_type(type)
{}

TreeItem::~TreeItem() = default;

QString TreeItem::name() const
{
    return m_name;
}

void TreeItem::setName(QString newName)
{
    m_name = newName;
}

std::weak_ptr<Folder> TreeItem::folder() const
{
    return m_folder;
}

TreeItem::Type TreeItem::type() const
{
    return m_type;
}

void TreeItem::setFolder(const std::shared_ptr<Folder> &newFolder)
{
    m_folder = newFolder;
}

}  // namespace feedling
