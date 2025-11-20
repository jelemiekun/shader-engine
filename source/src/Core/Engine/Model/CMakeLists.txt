message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(Model "${CMAKE_CURRENT_LIST_DIR}/Model.cpp")
target_include_directories(Model PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET Model)
  message(STATUS "Target Model successfully created.")
else()
  message(WARNING "Target Model failed to create.")
endif()
