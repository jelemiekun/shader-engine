message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(Camera "${CMAKE_CURRENT_LIST_DIR}/Camera.cpp")
target_include_directories(Camera PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Camera)
  message(STATUS "Target Camera successfully created.")
else()
  message(WARNING "Target Camera failed to create.")
endif()
