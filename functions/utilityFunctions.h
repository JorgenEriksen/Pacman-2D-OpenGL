#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../class/map.h"
#include <vector>
#include <string>
#include <iostream>

std::vector<int> getMapTileSize();
std::vector<float> getTileOffset();
std::vector<int> getLevelVector();
std::vector<std::vector<int>> getMapIndexData();

void CleanVAO(GLuint& vao);
double getDeltaTime();
std::vector<std::vector<glm::vec2>> getMapCordinates();
glm::vec3 randomGhostPosition(int seed);
glm::vec3 getPacmanStartingPosition();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
