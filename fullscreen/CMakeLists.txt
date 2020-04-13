set(FULLSCREEN_TARGET panda-launcher)

set(SOURCES applicationadaptor.cpp
            apprunthread.cpp
            appsmanager.cpp
            calcutil.cpp
            desktopinfo.h
            desktopproperties.cpp
            fullscreenframe.cpp
            itemdelegate.cpp
            listmodel.cpp
            listview.cpp
            main.cpp
            searchedit.cpp
            utils.cpp)
set(RESOURCES resources.qrc)

add_executable(${FULLSCREEN_TARGET} ${SOURCES} ${RESOURCES})
target_link_libraries(${FULLSCREEN_TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::DBus
        Qt5::Concurrent
        KF5::WindowSystem
)

install(TARGETS ${FULLSCREEN_TARGET} DESTINATION /usr/bin)