set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(test_data.hpp.in test_data.hpp)

add_executable(test_parser
    test_parser.cpp
    ../AtomParser.cpp
    ../Entry.cpp
    ../Feed.cpp
    ../Folder.cpp
    ../FeedParser.cpp
    ../RSSParser.cpp
    ../TreeItem.cpp)
target_link_libraries(test_parser PRIVATE test_base Qt5::Core)

add_test(NAME test_parser COMMAND test_parser)
