#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

TEMPLATE = subdirs
CONFIG += ordered
CONFIG += release

lessThan(QT_MAJOR_VERSION, 6):lessThan(QT_MINOR_VERSION, 12) {
    warning("Tests are only enabled on Qt 5.12.0 or later version. You are using $$[QT_VERSION].")
    DEFINES += WITHOUT_TESTS
}
android: DEFINES += WITHOUT_TESTS


contains(DEFINES, WITHOUT_TESTS) {
    SUBDIRS += quicknanobrowser \
        TestOnlyC \
        TestQMLWidgets \
        TestQtWidgets \
        virtualkeyboard \
        webui
}
