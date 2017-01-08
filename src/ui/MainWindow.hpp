#ifndef FEEDLING_UI_MAINWINDOW_HPP
#define FEEDLING_UI_MAINWINDOW_HPP

#include "../View.hpp"

#include <QtWidgets/QMainWindow>

class QListView;
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

    void init() override;
    void setFeedsModel(QAbstractItemModel *model) override;
    void setEntriesModel(QAbstractItemModel *model) override;

private:
    QTreeView *m_feeds;
    QListView *m_entries;
    QTextEdit *m_entryView;
};

}  // namespace ui
}  // namespace feedling

#endif  // FEEDLING_UI_MAINWINDOW_HPP
