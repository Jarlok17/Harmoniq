set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Widgets Qml Quick QuickControls2 REQUIRED)

set(PROJECT_SOURCES
    qml.qrc
    ToolBars/icons/icons.qrc
)

add_library(qml OBJECT
    ${PROJECT_SOURCES}
)

target_link_libraries(qml PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::QuickControls2)
