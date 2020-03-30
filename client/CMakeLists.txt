cmake_minimum_required (VERSION 2.8)

set (CMAKE_CXX_FLAGS "-g -std=c++11")

find_package(Boost 1.55 COMPONENTS system thread log REQUIRED)

include_directories(
    ../include/
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../prebuilt/x86_64/vsomeip/
    ${CMAKE_CURRENT_SOURCE_DIR}/../prebuilt/x86_64/boost/
)

add_executable(someip-client ../src/main.cpp)
target_link_libraries(someip-client pthread vsomeip3 boost_log)