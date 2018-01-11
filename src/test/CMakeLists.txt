add_executable(test_parser
    test_parser.cpp
    ../Entry.cpp
    ../Feed.cpp
    ../Folder.cpp
    ../FeedParser.cpp
    ../TreeItem)
target_link_libraries(test_parser PRIVATE test_base Qt5::Core)

add_test(NAME test_parser COMMAND test_parser)
