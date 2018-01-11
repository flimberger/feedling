set(feedling_SRCS
    feedling.cpp
    Application.cpp
    Entry.cpp
    EntriesModel.cpp
    Feed.cpp
    FeedParser.cpp
    FeedsModel.cpp
    Fetcher.cpp
    Folder.cpp
    Presenter.cpp
    TextFileDataStore.cpp
    TreeItem.cpp
    View.cpp
    ui/MainWindow.cpp
)

add_executable(feedling ${feedling_SRCS})
target_link_libraries(feedling
    Qt5::Network
    Qt5::Widgets
)

add_subdirectory(test)
