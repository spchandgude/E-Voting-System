cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CSS_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(OnlineVotingSystem)
file(GLOB SOURCES "*.cpp")
add_executable(cmakeexecutable ${SOURCES})


target_link_libraries(cmakeexecutable -lpthread -lsqlite3)