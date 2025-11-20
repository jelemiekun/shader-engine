message(STATUS "Loading file ${CMAKE_CURRENT_LIST_FILE}")

add_library(Shader "${CMAKE_CURRENT_LIST_DIR}/Shader.cpp")
target_include_directories(Shader PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Shader)
  message(STATUS "Target Shader successfully created.")
else()
  message(WARNING "Target Shader failed to create.")
endif()
