#include "MainWindow.hpp"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>

#include "../Presenter.hpp"

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

    QObject::connect(m_feeds, &QTreeView::clicked, this, &MainWindow::onFeedClicked);
    QObject::connect(m_entries, &QListView::clicked, this, &MainWindow::onEntryClicked);
}

MainWindow::~MainWindow() = default;

void MainWindow::init(Presenter *presenter)
{
    m_presenter = presenter;
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

void MainWindow::onFeedClicked(const QModelIndex &idx)
{
    m_presenter->selectFeed(idx);
}

void MainWindow::onEntryClicked(const QModelIndex &idx)
{
    m_presenter->selectEntry(idx);
}

}  // namespace ui
}  // namespace feedling
