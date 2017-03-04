#include <memory>

#include <QtWidgets/QApplication>

#include "Application.hpp"
#include "FileTreeStorage.hpp"
#include "ui/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("feedling");
    app.setOrganizationName("Purplekraken Software");
    app.setOrganizationDomain("purplekraken.com");
    auto storageRoot = QDir{".local/share/feedling"};
    feedling::Application feedling{std::make_unique<feedling::ui::MainWindow>(),
            std::make_unique<feedling::FileTreeStorage>(storageRoot)};

    feedling.onFetchFeeds();

    return app.exec();
}
