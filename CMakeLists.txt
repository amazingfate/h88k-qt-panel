cmake_minimum_required(VERSION 3.16)
project(h88k-qt-panel)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)


set(CMAKE_AUTOMOC ON)

add_executable(h88k-qt-panel main.cpp)
target_link_libraries(h88k-qt-panel Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
