set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Widgets Qml Quick QuickControls2 OpenGL REQUIRED)

set(PROJECT_SOURCES
    ToolHandler.hpp
    BrushTool.hpp
    EraseTool.hpp
    FillTool.hpp
    ColorPicker.hpp
    ShapeTool.hpp
    LineTool.hpp
    MoveTool.hpp
    SelectionTool.hpp
    ToolSettings.hpp
    ToolSettings.cpp
)

add_library(tools OBJECT
    ${PROJECT_SOURCES}
)

target_link_libraries(tools PRIVATE Qt6::Core Qt6::Widgets Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::OpenGL)
