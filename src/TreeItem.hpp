#ifndef FEEDLING_TREEITEM_HPP
#define FEEDLING_TREEITEM_HPP

#include <memory>

#include <QtCore/QString>

namespace feedling {

class Folder;

class TreeItem
{
public:
    enum class Type {
        FEED,
        FOLDER
    };

    virtual ~TreeItem();

    QString name() const;
    void setName(QString newName);

    Folder *folder();
    const Folder *folder() const;
    void setFolder(Folder *newFolder);

    Type type() const;

protected:
    TreeItem(QString name, Type type);

private:
    QString m_name;
    Folder *m_folder;
    Type m_type;
};

}  // namespace feedling

#endif  // FEEDLING_TREEITEM_HPP
