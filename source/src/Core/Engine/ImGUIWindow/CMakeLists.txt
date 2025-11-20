message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(ImGUIWindow "${CMAKE_CURRENT_LIST_DIR}/ImGUIWindow.cpp")
target_include_directories(ImGUIWindow PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET ImGUIWindow)
  message(STATUS "Target ImGUIWindow successfully created.")
else()
  message(WARNING "Target ImGUIWindow failed to create.")
endif()
