cmake_minimum_required(VERSION 3.5)

set(TARGET QtSingleApplication)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRCS
        ${CMAKE_CURRENT_LIST_DIR}/qtlocalpeer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/qtlocalpeer.h
        ${CMAKE_CURRENT_LIST_DIR}/QtLockedFile
        ${CMAKE_CURRENT_LIST_DIR}/qtlockedfile.cpp
        ${CMAKE_CURRENT_LIST_DIR}/qtlockedfile.h
        ${CMAKE_CURRENT_LIST_DIR}/QtSingleApplication
        ${CMAKE_CURRENT_LIST_DIR}/qtsingleapplication.cpp
        ${CMAKE_CURRENT_LIST_DIR}/qtsingleapplication.h
        ${CMAKE_CURRENT_LIST_DIR}/qtsinglecoreapplication.cpp
        ${CMAKE_CURRENT_LIST_DIR}/qtsinglecoreapplication.h)

if (WIN32)
    set(SRCS ${SRCS} qtlockedfile_win.cpp)
else()
    set(SRCS ${SRCS} qtlockedfile_unix.cpp)
endif ()

set(QT Core Gui Widgets Network)
find_package(Qt5 REQUIRED ${QT})
add_library(${TARGET} STATIC ${SRCS})
target_link_libraries(${TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
)
