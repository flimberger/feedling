#ifndef FEEDLING_FEEDMODELITEM_HPP
#define FEEDLING_FEEDMODELITEM_HPP

#include <memory>

#include <QtCore/QString>

namespace feedling {

class Folder;

class FeedsModelItem
{
public:
    enum class Type {
        FEED,
        FOLDER
    };
    virtual ~FeedsModelItem();

    QString name() const;
    void setName(QString newName);

    std::weak_ptr<Folder> folder() const;
    Type type() const;

protected:
    FeedsModelItem(QString name, Type type);
    void setFolder(const std::shared_ptr<Folder> &newFolder);

private:
    QString m_name;
    std::weak_ptr<Folder> m_folder;
    Type m_type;
};

}  // namespace feedling

#endif  // FEEDLING_FEEDMODELITEM_HPP
