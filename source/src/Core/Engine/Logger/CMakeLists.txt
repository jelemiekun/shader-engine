message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(Logger "${CMAKE_CURRENT_LIST_DIR}/Logger.cpp")
target_include_directories(Logger PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Logger)
  message(STATUS "Target Logger successfully created.")
else()
  message(WARNING "Target Logger failed to create.")
endif()
