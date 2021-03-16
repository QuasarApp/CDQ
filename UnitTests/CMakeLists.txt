#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.1)
set(CURRENT_PROJECT ${PROJECT_NAME}Test)

include(../QuasarAppLib/CMake/ProjectOut.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

file(GLOB SOURCE_CPP
    "*.cpp" "*.qrc"
)

include(../../QuasarAppLib/CMake/ProjectOut.cmake)


add_definitions(-DQTEST_FUNCTION_TIMEOUT=900000)
add_definitions(-DQT_BASE_DIR="${Qt5_DIR}")
add_definitions(-DTEST_BIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${CURRENT_PROJECT} ${SOURCE_CPP})
target_link_libraries(${CURRENT_PROJECT} PRIVATE Deploy Qt::Test)
target_include_directories(${CURRENT_PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
