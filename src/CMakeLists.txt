set(feedling_SRCS
    feedling.cpp
    Application.cpp
    DownloadTask.cpp
    Entry.cpp
    EntriesModel.cpp
    Feed.cpp
    FeedParser.cpp
    FeedsModel.cpp
    FileTreeStorage.cpp
    Folder.cpp
    IStorage.cpp
    Presenter.cpp
    TreeItem.cpp
    View.cpp
    ui/MainWindow.cpp
)

add_executable(feedling ${feedling_SRCS})
target_link_libraries(feedling
    Qt5::Network
    Qt5::Widgets
)
