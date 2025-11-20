message(STATUS "Loading file ${CMAKE_CURRENT_LIST_FILE}")

add_library(VertexArray "${CMAKE_CURRENT_LIST_DIR}/VertexArray.cpp")
target_include_directories(VertexArray PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET VertexArray)
  message(STATUS "Target VertexArray successfully created.")
else()
  message(WARNING "Target VertexArray failed to create.")
endif()
