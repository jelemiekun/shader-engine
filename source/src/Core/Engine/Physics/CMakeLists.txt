message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

add_library(Physics "${CMAKE_CURRENT_LIST_DIR}/Physics.cpp" "${CMAKE_CURRENT_LIST_DIR}/RigidBody.cpp")
target_include_directories(Physics PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../../../../include/Core/Engine")

if(UNIX)
    target_include_directories(Physics PUBLIC /usr/include/bullet)
elseif(WIN32)
  target_include_directories(Physics PUBLIC ${BULLET_INCLUDE_DIRS})
  target_include_directories(Physics PUBLIC "C:/vcpkg/installed/x64-windows/include" "C:/vcpkg/installed/x64-windows/include/bullet" ${BULLET_INCLUDE_DIRS})
  link_directories("C:/vcpkg/installed/x64-windows/lib")
endif()


if (TARGET Physics)
  message(STATUS "Target Physics successfully created.")
else()
  message(WARNING "Target Physics failed to create.")
endif()
