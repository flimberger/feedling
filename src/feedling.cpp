#include <memory>

#include <QtWidgets/QApplication>

#include "Application.hpp"
#include "ui/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    feedling::Application feedling{std::make_unique<feedling::ui::MainWindow>()};

    feedling.onFetchFeeds();

    return app.exec();
}
