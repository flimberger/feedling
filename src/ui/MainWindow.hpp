#ifndef FEEDLING_UI_MAINWINDOW_HPP
#define FEEDLING_UI_MAINWINDOW_HPP

#include "../View.hpp"

#include <QtWidgets/QMainWindow>

class QListView;
class QTextDocument;
class QTextEdit;
class QTreeView;

namespace feedling {

namespace ui {

class MainWindow : public QMainWindow, public View
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();

    void init(Presenter *presenter) override;
    void setFeedsModel(QAbstractItemModel *model) override;
    void setEntriesModel(QAbstractItemModel *model) override;
    void showEntry(const std::shared_ptr<Entry> &entry) override;

private:
    Q_SLOT void onFeedClicked(const QModelIndex &idx);
    Q_SLOT void onEntryClicked(const QModelIndex &idx);

    std::unique_ptr<QTextDocument> createEntryView(const std::shared_ptr<Entry> &entry) const;

    QTreeView *m_feeds;
    QListView *m_entries;
    QTextEdit *m_entryView;
    Presenter *m_presenter;
};

}  // namespace ui
}  // namespace feedling

#endif  // FEEDLING_UI_MAINWINDOW_HPP
