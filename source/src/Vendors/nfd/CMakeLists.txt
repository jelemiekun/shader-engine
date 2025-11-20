message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

set (nfd_src "${CMAKE_CURRENT_LIST_DIR}/nfd_common.c")

if (WIN32)
  list(APPEND nfd_src "${CMAKE_CURRENT_LIST_DIR}/nfd_win.cpp")
elseif(UNIX)
  list(APPEND nfd_src "${CMAKE_CURRENT_LIST_DIR}/nfd_gtk.c") # nfd_gtk.c or nfd_zenity.c , choose one
elseif(APPLE)
  list(APPEND nfd_src "${CMAKE_CURRENT_LIST_DIR}/nfd_cocoa.m")
endif()

add_library(nfd ${nfd_src})
target_include_directories(nfd PUBLIC 
  "${CMAKE_CURRENT_LIST_DIR}/../../../include/Vendors/nfd"
)

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_include_directories(nfd PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nfd PRIVATE ${GTK3_LIBRARIES})
    target_compile_options(nfd PRIVATE ${GTK3_CFLAGS_OTHER})
endif()

if (TARGET nfd)
  message(STATUS "Target nfd successfully created.")
else()
  message(WARNING "Target nfd failed to create.")
endif()
