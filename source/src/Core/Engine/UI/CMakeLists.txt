message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(UI "${CMAKE_CURRENT_LIST_DIR}/UI.cpp")
target_include_directories(UI PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if (TARGET UI)
  message(STATUS "Target UI successfully created.")
else()
  message(WARNING "Target UI failed to create.")
endif()
