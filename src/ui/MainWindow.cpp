#include "MainWindow.hpp"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>

namespace feedling {
namespace ui {

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{parent},
    View{},
    m_feeds{new QTreeView{this}},
    m_entries{new QListView{this}},
    m_entryView{ new QTextEdit{this}}
{
    auto *layout = new QGridLayout;
    layout->addWidget(m_feeds, 0, 0, -1, 1);
    layout->addWidget(m_entries, 0, 1);
    m_entryView->setReadOnly(true);
    layout->addWidget(m_entryView, 1, 1);
    auto *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow() = default;

void MainWindow::init()
{
    show();
}

void MainWindow::setFeedsModel(QAbstractItemModel *model)
{
    m_feeds->setModel(model);
}

void MainWindow::setEntriesModel(QAbstractItemModel *model)
{
    m_entries->setModel(model);
}

}  // namespace ui
}  // namespace feedling
