set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Widgets Qml Quick QuickControls2 OpenGL REQUIRED)

set(PROJECT_SOURCES
    platform.hpp
)

set(PCH_HEADER precompiled.hpp)

add_library(configs OBJECT
    ${PROJECT_SOURCES}
)

target_precompile_headers(configs PRIVATE ${PCH_HEADER})

target_link_libraries(configs PRIVATE Qt6::Core Qt6::Widgets Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::OpenGL)

target_include_directories(configs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
