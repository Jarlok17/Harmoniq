add_subdirectory(ui)
add_subdirectory(layer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Qml Quick QuickControls2)

set(PROJECT_SOURCES
    Application.cpp
    core.qrc
    styles/StyleResources.qrc
    shaders/shaders.qrc
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::QuickControls2 ui layer)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "Harmoniq"
)

qt_finalize_executable(${PROJECT_NAME})

