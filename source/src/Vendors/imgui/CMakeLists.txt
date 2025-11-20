message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(imgui 
  ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC 
  "${CMAKE_CURRENT_LIST_DIR}/../../../include/Vendors/imgui"
  "${CMAKE_CURRENT_LIST_DIR}/../../../include/Vendors/imgui/backends"
)

if (TARGET imgui)
  message(STATUS "Target imgui successfully created.")
else()
  message(WARNING "Target imgui failed to create.")
endif()
