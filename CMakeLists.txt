cmake_minimum_required(VERSION 3.5)
project(panda-launcher)

set(TARGET panda-launcher)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file (GLOB_RECURSE SRCS
  "${panda-launcher_SOURCE_DIR}/src/*.cpp"
)

set(SOURCES ${SRCS})
set(HEADERS "")

set(FORMS "")
set(RESOURCES src/resources.qrc)

set(QT Core Gui Widgets Network Concurrent)
find_package(Qt5 REQUIRED ${QT})
find_package(PkgConfig REQUIRED)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
find_package(KF5 REQUIRED COMPONENTS WindowSystem)
add_subdirectory(QtSingleApplication)
add_executable(${TARGET} ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})
target_include_directories(${TARGET} PRIVATE QtSingleApplication)
target_link_libraries(${TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Concurrent
        QtSingleApplication
        KF5::WindowSystem
)

install(TARGETS ${TARGET} DESTINATION /usr/bin)
