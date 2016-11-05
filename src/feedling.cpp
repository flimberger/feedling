#include <QtWidgets/QApplication>

#include "Application.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    feedling::Application feedling;

    feedling.onFetchFeeds();

    return app.exec();
}
