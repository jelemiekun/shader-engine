message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(glad "${CMAKE_CURRENT_LIST_DIR}/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../include/Vendors/glad")

if (TARGET glad)
  message(STATUS "Target Game successfully created.")
else()
  message(WARNING "Target Game failed to create.")
endif()
