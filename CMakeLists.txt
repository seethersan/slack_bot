cmake_minimum_required(VERSION 3.8)
project(slack_bot)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(cpprestsdk REQUIRED)
include_directories(/usr/local/Cellar/boost/1.65.1/include)
include_directories(/usr/local/opt/sqlite/include)

find_package(Boost COMPONENTS chrono system thread)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp connection.cpp connection.h db.cpp db.h user.cpp user.h team.cpp team.h)
add_executable(slack_bot ${SOURCE_FILES})
target_link_libraries(slack_bot
        PRIVATE cpprestsdk::cpprest
        ${Boost_LIBRARIES}
        /usr/local/opt/sqlite/lib/libsqlite3.dylib)