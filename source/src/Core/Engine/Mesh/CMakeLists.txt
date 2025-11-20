message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(Mesh "${CMAKE_CURRENT_LIST_DIR}/Mesh.cpp")
target_include_directories(Mesh PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Mesh)
  message(STATUS "Target Mesh successfully created.")
else()
  message(WARNING "Target Mesh failed to create.")
endif()
