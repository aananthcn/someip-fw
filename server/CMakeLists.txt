cmake_minimum_required (VERSION 2.8)

set (CMAKE_CXX_FLAGS "-g -std=c++11")

# Define the cross compiler locations
set (CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
set (CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)


#find_package(vsomeip 2.6.0 REQUIRED)
find_package(Boost 1.55 COMPONENTS system thread log REQUIRED)

include_directories(
    ../include/
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../prebuilt/armv7l/vsomeip/
    ${CMAKE_CURRENT_SOURCE_DIR}/../prebuilt/armv7l/boost/
)

add_executable(someip-service ../src/main.cpp)
target_link_libraries(someip-service vsomeip3 boost_log)