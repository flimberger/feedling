set(feedling_SRCS
    feedling.cpp
    Application.cpp
    Entry.cpp
    EntriesModel.cpp
    Feed.cpp
    FeedParser.cpp
    FeedsModel.cpp
    FeedsModelItem.cpp
    Folder.cpp
    ../res/feedling.qrc
)

add_executable(feedling ${feedling_SRCS})
target_link_libraries(feedling
    Qt5::Network
    Qt5::Quick
    Qt5::Widgets
)
