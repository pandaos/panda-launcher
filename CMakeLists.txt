cmake_minimum_required(VERSION 3.5)
project(panda-launcher)
set(TARGET panda-launcher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT Core Gui Widgets DBus Network Concurrent)
find_package(Qt5 REQUIRED ${QT})
find_package(PkgConfig REQUIRED)
find_package(KF5WindowSystem REQUIRED)

set(SRCS    src/applicationadaptor.cpp
            src/apprunthread.cpp
            src/appsmanager.cpp
            src/calcutil.cpp
            src/desktopinfo.h
            src/desktopproperties.cpp
            src/fullscreenframe.cpp
            src/itemdelegate.cpp
            src/listmodel.cpp
            src/listview.cpp
            src/main.cpp
            src/utils.cpp)
set(RESOURCES src/resources.qrc)

add_executable(${TARGET} ${SRCS} ${RESOURCES})
target_link_libraries(${TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::DBus
        Qt5::Concurrent
        KF5::WindowSystem
)

install(TARGETS ${TARGET} DESTINATION /usr/bin)
