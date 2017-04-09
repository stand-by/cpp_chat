cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

project(cpp_chat)

set(SOURCE_LIB JsonWrapper.cpp)
add_library(JsonWrapper STATIC ${SOURCE_LIB})
set(SOURCE_LIB MessageThread.cpp)
add_library(MessageThread STATIC ${SOURCE_LIB})

add_executable(main main.cpp)

target_link_libraries(main JsonWrapper)
target_link_libraries(main MessageThread)
