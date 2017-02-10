set(feedling_SRCS
    feedling.cpp
    Application.cpp
    Entry.cpp
    EntriesModel.cpp
    Feed.cpp
    FeedParser.cpp
    FeedsModel.cpp
    Folder.cpp
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

enable_testing()
find_package(GTest REQUIRED)
add_executable(test_parser test_parser.cpp FeedParser.cpp TreeItem.cpp Feed.cpp Entry.cpp)
target_link_libraries(test_parser
    GTest::GTest
    GTest::Main
    Qt5::Core)
add_test(NAME test_parser COMMAND test_parser)
