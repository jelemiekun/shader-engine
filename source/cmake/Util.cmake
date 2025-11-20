message(STATUS "Loading ${CMAKE_CURRENT_LIST_FILE}")

function(setup_standard_project)
  message(STATUS "Setting up standard project...")

  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

  if (WIN32 AND MSVC)
      add_compile_options("/utf-8")
  endif()

  message(STATUS "Standard project setup done.")
endfunction()

function(find_packages)
  message(STATUS "Finding packages: ${ARGV}")

  foreach(package IN ITEMS ${ARGV})
    set (success TRUE)
    if(UNIX)
        message(STATUS "Finding ${package} package for linux operating system...")
        find_package(${package} REQUIRED)
    elseif(WIN32)
        message(STATUS "Finding ${package} package for windows operating system...")

        if(DEFINED ENV{VCPKG_ROOT})
            list(APPEND CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/x64-windows")
        else()
          message(WARNING "VCPKG_ROOT not defined. Please set it (setx VCPKG_ROOT \"C:/path/to/vcpkg\" or pass the toolchain file ().") 
        endif()

        find_package(${package} CONFIG REQUIRED)
    else()
        message(STATUS "Current operating system unknown...")

        set (success FALSE)
    endif()

    if (success)
      message(STATUS "Package ${package} found.")
    endif()
  endforeach()

  message(STATUS "Packages finding process done.")
endfunction()

function(compile_definitions)
  message(STATUS "Creating compile definitions...")
  add_compile_definitions(ASSET_PATH="${CMAKE_SOURCE_DIR}/../assets/")
  add_compile_definitions(CMAKE_SOURCE_PATH="${CMAKE_SOURCE_DIR}")
  message(STATUS "Compile definitions created.")
endfunction()

function(create_source_libraries)
  message(STATUS "Creating source libraries...")

  set(ENGINE_DIRS
    src/Core/Engine/Camera
    src/Core/Engine/ElementBuffer
    src/Core/Engine/Engine
    src/Core/Engine/Logger
    src/Core/Engine/Mesh
    src/Core/Engine/Model
    src/Core/Engine/Physics
    src/Core/Engine/Shader
    src/Core/Engine/Texture2D
    src/Core/Engine/UI
    src/Core/Engine/VertexArray
    src/Core/Engine/VertexBuffer
    src/Vendors/stb_image
    src/Vendors/glad
    src/Vendors/imgui
    src/Vendors/nfd
  )

  foreach(dir ${ENGINE_DIRS})
    add_subdirectory("${CMAKE_SOURCE_DIR}/${dir}")
  endforeach()

  message(STATUS "Source libraries created.")
endfunction()

function(create_executables)
  message(STATUS "Creating executables...")

  add_executable(ShaderExe "${CMAKE_SOURCE_DIR}/src/Core/main.cpp")

  message(STATUS "Executabled created.")
endfunction()

function(link_all)
  message(STATUS "Linking all targets...")

  message(STATUS "Bullet include dirs: ${BULLET_INCLUDE_DIRS}")
  message(STATUS "Bullet libraries: ${BULLET_LIBRARIES}")

  target_link_libraries(ShaderExe PUBLIC spdlog::spdlog SDL2::SDL2 Engine)

  target_link_libraries(Engine PUBLIC SDL2::SDL2 glad UI Physics Logger)
  target_link_libraries(Camera PUBLIC SDL2::SDL2 glad glm::glm)
  target_link_libraries(imgui PUBLIC SDL2::SDL2)
  target_link_libraries(Mesh PUBLIC assimp::assimp glm::glm glad Shader )
  target_link_libraries(Model PUBLIC glm::glm glad stb_image assimp::assimp Mesh)
  target_link_libraries(Shader PUBLIC glad glm::glm)
  target_link_libraries(Texture2D PUBLIC stb_image glad glm::glm)
  target_link_libraries(UI PUBLIC SDL2::SDL2 glad imgui nfd)
  target_link_libraries(VertexBuffer PUBLIC glad)
  target_link_libraries(VertexArray PUBLIC glad)

  if (UNIX)
    target_link_libraries(ElementBuffer PUBLIC glad)
  elseif(WIN32)
    target_link_libraries(ElementBuffer PUBLIC glad assimp::assimp)
  endif()

  if (WIN32)
    target_link_libraries(Logger PUBLIC spdlog::spdlog_header_only)
  endif()

  if (UNIX)
    target_link_libraries(Physics PUBLIC BulletDynamics BulletCollision LinearMath)
  elseif(WIN32)
    target_link_libraries(Physics PUBLIC
	$<$<CONFIG:Debug>:BulletCollision BulletDynamics LinearMath>
        $<$<CONFIG:Release>:BulletCollision BulletDynamics LinearMath>
    )
  endif()

  message(STATUS "All targets linked.")
endfunction()
