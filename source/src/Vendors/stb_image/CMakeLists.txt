message(STATUS "Loading file ${CMAKE_CURRENT_LIST_FILE}")

add_library(stb_image "${CMAKE_CURRENT_LIST_DIR}/stb_image.cpp")
target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../include/Vendors")

if (TARGET stb_image)
  message(STATUS "Target stb_image successfully created.")
else()
  message(WARNING "Target stb_image failed to create.")
endif()
