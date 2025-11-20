message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(VertexBuffer "${CMAKE_CURRENT_LIST_DIR}/VertexBuffer.cpp")
target_include_directories(VertexBuffer PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET VertexBuffer)
  message(STATUS "Target VertexBuffer successfully created.")
else()
  message(WARNING "Target VertexBuffer failed to create.")
endif()
