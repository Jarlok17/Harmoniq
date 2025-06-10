add_subdirectory(qml)
add_subdirectory(layer)
add_subdirectory(config)
add_subdirectory(document)
add_subdirectory(tools)
add_subdirectory(history)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PRECOMPILE_HEADERS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Qml Quick QuickControls2)
find_package(Qt6 COMPONENTS OpenGL QUIET)
if (Qt6OpenGL_FOUND)
    message("Qt6 OpenGL found, enabling OpenGL support")
else()
    message("Qt6 OpenGL not found, OpenGL support disabled")
endif()

set(PROJECT_SOURCES
    Application.cpp
    core.qrc
    styles/StyleResources.qrc
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE config/precompiled.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    $<$<BOOL:${Qt6OpenGL_FOUND}>:Qt6::OpenGL>
    qml
    layer
    configs
    document
    tools
    history
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "Harmoniq"
)

qt_finalize_executable(${PROJECT_NAME})
