# Shader Engine

## About The Project
This project was inspired by focusing on shader programming, hence the project name **"Shader Engine"**.

## Features
- 

## Future Updates
- One default camera for viewing
  - Controls:
    - MOUSE_LEFT_CLICK: select
    - MOUSE_MIDDLE_CLICK: pan around the center
    - MOUSE_MIDDLE_CLICK + SHIFT: move around
    - MOUSE_RIGHT_CLICK: view around from the position
  - Render modes: 
    - RENDER_MODE_FLAT_OUTLINE: solid color + outline
    - RENDER_MODE_WIREFRAME: wireframe 
    - RENDER_MODE_UNLIT: textured, no lighting
    - RENDER_MODE_LIT: textured + lighting
- Upload models
  - Perform transformation
- Separate Window Class
- Engine state (for shortcut keys when in engine/in game)
- Upload your own skybox via folder selection containing the images
- Entry point for the actual game application
- Networking (for multiplayers)
- Ctrl+W = Set boolean false to currently selected imgui window
- Embedded scripting languages:
  - python (_via_ Pybind11)
  - lua (_via_ LuaJIT)
- Scripting editor for embedded scripting languages (_via_ ...)
- Upload your own background for game engine and set opacity
- Window options for each imgui window
- Change font size and color

## Built With

## Installation

## Build Source File
### Windows
- Download vcpkg
- Perform:
```bash
vcpkg install sdl2:x64-windows
vcpkg install glm:x64-windows
vcpkg install spdlog:x64-windows
vcpkg install assimp:x64-windows
vcpkg install bullet3:x64-windows

vcpkg integrate install
```
- Clone repo
- Perform:
```bash
cmake -S source -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/your/vcpkg.cmake
cmake --build build
```
  - Open ShaderEngine.sln with Visual Studio
  - Build ShaderExe project

### Unix
#### Debian Based
- Perform:
```bash
sudo apt update
sudo apt install libsdl2-dev libglm-dev libspdlog-dev libassimp-dev libbullet-dev
```
- Clone repo
- Perform:
```bash
cmake -S source -B build
cmake --build build
```

## Usage

## Contributing
