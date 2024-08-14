cmake_minimum_required(VERSION 3.23)

add_executable(search_engine main.cpp ConverterJSON.cpp InvertedIndex.cpp SearchServer.cpp)
target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

add_executable(run_tests test.cpp InvertedIndex.cpp SearchServer.cpp)
target_include_directories(run_tests PUBLIC ${INCLUDE_DIR})
target_link_libraries(run_tests PRIVATE gtest_main)
target_link_libraries(run_tests PRIVATE nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(run_tests)