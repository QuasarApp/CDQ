#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.1)

IF(NOT DEFINED WITHOUT_TESTS)

    find_program(Q_MAKE_EXE qmake)
    IF(NOT EXISTS ${Q_MAKE_EXE})
        message("the qmake not exits!")
        return()
    endif(NOT EXISTS ${Q_MAKE_EXE})
    message("QMake of the tests projects = ${Q_MAKE_EXE}")

    set(MAKE_CMD make)

    add_custom_target(
        QMAKE_TESTS
        COMMAND ${Q_MAKE_EXE} -r
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running ${Q_MAKE_EXE} -r"
    )

    add_custom_target(
        MAKE_TESTS ALL
        COMMAND ${CMAKE_MAKE_PROGRAM} -j4
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running ${CMAKE_MAKE_PROGRAM} -j4"
    )

    add_dependencies(MAKE_TESTS QMAKE_TESTS)


endif(NOT DEFINED WITHOUT_TESTS)


