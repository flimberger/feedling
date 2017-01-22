#include "MainWindow.hpp"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtWidgets/QListView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>

#include "../Entry.hpp"
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
    auto innerSplitter = std::make_unique<QSplitter>(Qt::Vertical);
    innerSplitter->addWidget(m_entries);
    innerSplitter->addWidget(m_entryView);
    auto outerSplitter = std::make_unique<QSplitter>();
    outerSplitter->addWidget(m_feeds);
    outerSplitter->addWidget(innerSplitter.release());
    setCentralWidget(outerSplitter.release());

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

void MainWindow::showEntry(const std::shared_ptr<Entry> &entry)
{
    m_entryView->setDocument(createEntryView(entry).release());
}

void MainWindow::onFeedClicked(const QModelIndex &idx)
{
    m_presenter->selectFeed(idx);
}

void MainWindow::onEntryClicked(const QModelIndex &idx)
{
    m_presenter->selectEntry(idx);
}

std::unique_ptr<QTextDocument> MainWindow::createEntryView(const std::shared_ptr<Entry> &entry) const
{
    auto doc = std::make_unique<QTextDocument>();
    auto crs = QTextCursor{doc.get()};
    crs.insertHtml(entry->content());
    return doc;
}

}  // namespace ui
}  // namespace feedling
