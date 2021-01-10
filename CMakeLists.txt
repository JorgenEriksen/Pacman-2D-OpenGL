# CMakeList.txt : CMake project for Assignment-1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Assignment-1")

find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory("external/glew-cmake-2.1.0")
add_subdirectory("external/glfw-3.3.2")
add_subdirectory("external/glm")


add_executable (
PacmanGame
"main.cpp"
 "functions/utilityFunctions.cpp"
  "functions/utilityFunctions.h"
shaders/square.h
  "class/map.h"
  "class/map.cpp" "class/GameObject.cpp" "class/GameObject.h" "class/MovingUnits.h" "class/MovingUnits.cpp" "class/Pacman.h" "class/Pacman.cpp" "global/global.h"  "class/Ghost.h" "class/Ghost.cpp" "class/Pellets.cpp" "class/Texture.h" "class/Texture.cpp" "class/stb_image.h" "class/stb_image.cpp" "class/Shader.h" "class/Shader.cpp"  "functions/pacmanStartingPos.h" "functions/pacmanStartingPos.cpp" )

target_compile_definitions(PacmanGame PRIVATE GLEW_STATIC)


target_link_libraries(PacmanGame
  PRIVATE
  libglew_static
  glfw
  glm
  OpenGL::GL)


target_include_directories(PacmanGame PUBLIC functions)
target_include_directories(PacmanGame PUBLIC shaders)
target_include_directories(PacmanGame PUBLIC class)
target_include_directories(PacmanGame PUBLIC global)