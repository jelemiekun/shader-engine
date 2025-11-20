message(STATUS "Loading file ${CMAKE_CURRENT_LIST_FILE}")

add_library(Texture2D "${CMAKE_CURRENT_LIST_DIR}/Texture2D.cpp")
target_include_directories(Texture2D PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Texture2D)
  message(STATUS "Target Texture2D successfully created.")
else()
  message(WARNING "Target Texture2D failed to create.")
endif()
