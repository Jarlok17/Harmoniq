set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Widgets Qml Quick QuickControls2 OpenGL REQUIRED)

set(PROJECT_SOURCES
    CanvasGL.hpp 
    CanvasGL.cpp
)

add_library(layer OBJECT
    ${PROJECT_SOURCES}
)

target_link_libraries(layer PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::QuickControls2 Qt6::OpenGL)
