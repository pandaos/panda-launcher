set(TARGET_NAME panda-keylauncher)

set(SOURCES main.cpp)
# set(RESOURCES resources.qrc)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::DBus
        Qt5::Concurrent
        KF5::WindowSystem
)

install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin)