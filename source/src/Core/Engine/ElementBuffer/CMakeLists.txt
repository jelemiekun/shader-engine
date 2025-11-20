message(STATUS "Loading file ${CMAKE_CURRENT_LIST_FILE}")

add_library(ElementBuffer "${CMAKE_CURRENT_LIST_DIR}/ElementBuffer.cpp")
target_include_directories(ElementBuffer PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET ElementBuffer)
  message(STATUS "Target ElementBuffer successfully created.")
else()
  message(WARNING "Target ElementBuffer failed to create.")
endif()
