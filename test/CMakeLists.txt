enable_testing()

find_package(GTest REQUIRED)

set(test_parser_SRCS
    test_parser.cpp
    ../src/FeedParser.cpp
    ../src/TreeItem.cpp
    ../src/Feed.cpp
    ../src/Entry.cpp)
add_executable(test_parser ${test_parser_SRCS})
target_include_directories(test_parser PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(test_parser
    GTest::GTest
    GTest::Main
    Qt5::Core)
add_test(NAME test_parser COMMAND test_parser)
