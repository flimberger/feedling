#include "FeedsModelItem.hpp"

#include "Folder.hpp"

namespace feedling {

FeedsModelItem::FeedsModelItem(QString name, Type type)
  : m_name(name),
    m_type(type)
{}

FeedsModelItem::~FeedsModelItem() = default;

QString FeedsModelItem::name() const
{
    return m_name;
}

void FeedsModelItem::setName(QString newName)
{
    m_name = newName;
}

std::weak_ptr<Folder> FeedsModelItem::folder() const
{
    return m_folder;
}

FeedsModelItem::Type FeedsModelItem::type() const
{
    return m_type;
}

void FeedsModelItem::setFolder(const std::shared_ptr<Folder> &newFolder)
{
    m_folder = newFolder;
}

}  // namespace feedling
