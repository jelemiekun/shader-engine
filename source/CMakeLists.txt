cmake_minimum_required(VERSION 3.20)

project(GamePrac VERSION 1.0.0.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(packages SDL2 spdlog glm assimp Bullet)

message(STATUS "Appending ${CMAKE_SOURCE_DIR}/cmake to CMAKE_MODULE_PATH")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Util)

setup_standard_project()
find_packages(${packages})
compile_definitions()
create_source_libraries()
create_executables()
link_all()
