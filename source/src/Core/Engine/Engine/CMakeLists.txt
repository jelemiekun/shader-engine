message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(Engine "${CMAKE_CURRENT_LIST_DIR}/Engine.cpp")
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Engine)
  message(STATUS "Target Engine successfully created.")
else()
  message(WARNING "Target Engine failed to create.")
endif()
