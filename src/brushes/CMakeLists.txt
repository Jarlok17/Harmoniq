set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Widgets Qml Quick QuickControls2 REQUIRED)

set(PROJECT_SOURCES
    BrushInfo.hpp
)

add_library(brushes OBJECT
    ${PROJECT_SOURCES}
)

target_link_libraries(brushes PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::QuickControls2)
