#ifndef FEEDLING_TREEITEM_HPP
#define FEEDLING_TREEITEM_HPP

#include <memory>
#include <vector>

#include <QtCore/QString>

namespace feedling {

class Feed;
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

class FeedItem : public TreeItem {
public:
    explicit FeedItem(const std::shared_ptr<Feed> &feed);
    ~FeedItem();

    const Feed *data() const;
    std::shared_ptr<Feed> data();

private:
    std::shared_ptr<Feed> m_feed;
};

}  // namespace feedling

#endif  // FEEDLING_TREEITEM_HPP
