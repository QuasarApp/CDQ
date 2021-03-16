#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.1)

project(CQtDeployer LANGUAGES CXX)
set(BUILD_SHARED_LIBS ON)

include(QuasarAppLib/CMake/ccache.cmake)
include(QuasarAppLib/CMake/QuasarAppCITargets.cmake)

initAll()
addDoc(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)

# Add sub directories
add_subdirectory(QuasarAppLib)
add_subdirectory(pe)
add_subdirectory(zip)

add_subdirectory(src)
add_subdirectory(UnitTests)

