cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(PROJECT_NAME NeoArt)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

project(${PROJECT_NAME})

add_subdirectory(src)
include_directories(src)

#set(VERSION "0.1.0")
#set(EXECUTABLE_PATH "${CMAKE_BINARY_DIR}/bin/NeoArt")
#set(ICON_PATH "${CMAKE_SOURCE_DIR}/src/resources/icons/artwave.ico")

#configure_file(
#  ${CMAKE_SOURCE_DIR}/src/resources/neoart.desktop.in
#  ${CMAKE_BINARY_DIR}/neoart.desktop
#  @ONLY
#)

#if(EXISTS "${CMAKE_BINARY_DIR}/neoart.desktop")
#  message(STATUS "Файл neoart.desktop успішно створено в ${CMAKE_BINARY_DIR}.")
#else()
#  message(WARNING "Файл neoart.desktop не було створено!")
#endif()

#install(
#  FILES ${CMAKE_BINARY_DIR}/neoart.desktop
#  DESTINATION $ENV{HOME}/.local/share/applications
#)

#install(CODE "if(EXISTS \"$ENV{HOME}/.local/share/applications/neoart.desktop\")
#message(STATUS \"Файл neoart.desktop успішно скопійовано в ~/.local/share/applications.\")
#else()
#  message(WARNING \"Файл neoart.desktop не було скопійовано!\")
#endif()"
#)

