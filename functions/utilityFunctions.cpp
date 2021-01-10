/**
*   A collection of various functions used throughout the entire program.
*   These serve many purposes but at the same time dont fit very well into any particular class.
*   
*
*  @name utilityFunctions.cpp
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "utilityFunctions.h"
#include <windows.h>
#include <fstream>
#include <set>
#include <time.h>
#include <random>
#include <chrono>

/**
*   Retrives the size of the map from the level0 file.
* 
*   @return    vector with the size in x and y for the map.
*/

std::vector<int> getMapTileSize()
{
    std::ifstream inFile;
    inFile.open("../../../../levels/level0");
    if (!inFile) {
        std::cerr << "Unable to open file level0";
        exit(1);   // call system to stop
    }

    int tilesX;
    int tilesY;

    inFile >> tilesX;
    inFile.ignore();
    inFile >> tilesY;

    std::vector<int> tiles = { tilesX, tilesY };
    return tiles;
}

/**
*   Gives the size of the mid adjust based on a percentege of tile size.
* 
*  @return    vector with the correct dynamic offsets.
*  @see       getMapTileSize.
*/
std::vector<float> getTileOffset()
{
    std::vector<int> offsetData = getMapTileSize();
    float offsetX = 1.0f / offsetData[0];
    float offsetY = 1.0f / offsetData[1];
    std::vector<float> offsets = { offsetX, offsetY };
    return offsets;
    //std::vector<int> offset
}


/**
*   Converting the level0 file into a vector and reversing it.
* 
*  @return    vector of level0.
*/
std::vector<int> getLevelVector() {
    std::vector<int> map;
    std::ifstream inFile;
    
    inFile.open("../../../../levels/level0");
    if (!inFile) {
        std::cerr << "Unable to open file level0";
        exit(1);   // call system to stop
    }


    inFile.ignore(); inFile.ignore(); inFile.ignore();

    int n;

    while (inFile >> n) {
        map.push_back(n);
    }
    
    std::reverse(map.begin(), map.end()); // 
    return map;
}

/**
*   Uses map data and converts it into a 2d vector of tiles.
* 
*  @return    vector of 2d tiles.
*  @see       getMapIndexData, getLevelVector, getMapTileSize
*/
std::vector<std::vector<int>> getMapIndexData() {
    std::vector<int> levelData = getLevelVector();
    std::vector<std::vector<int>> levelArray;
    int tilesX = getMapTileSize()[0];
    int tilesY = getMapTileSize()[1];
    levelArray.resize(tilesX, std::vector<int>(tilesY));
    
    auto counter = 0;
    for (int i = 0; i < tilesY; i++) {
        for (int j = 0; j < tilesX; j++) {
            levelArray[j][i] = levelData[counter];
            counter++;
        }
    }
    return levelArray;
}


/**
*   Function that is used to manage the memory of vertex array objects.
* 
*  @param     vao - the object that is sent in with refrence to delete/free.
*/
void CleanVAO(GLuint& vao) {
    GLint nAttr = 0;
    std::set<GLuint> vbos;

    GLint eboId;
    glGetVertexArrayiv(vao, GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboId);
    glDeleteBuffers(1, (GLuint*)&eboId);

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
    glBindVertexArray(vao);

    for (int iAttr = 0; iAttr < nAttr; ++iAttr) {
        GLint vboId = 0;
        glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
        if (vboId > 0) {
            vbos.insert(vboId);
        }

        glDisableVertexAttribArray(iAttr);
    }

    for (auto vbo : vbos) {
        glDeleteBuffers(1, &vbo);
    }

    glDeleteVertexArrays(1, &vao);
}

/**
*   Creates a smooth movement that is based on delta time instead of the frames per second of window.
* 
*   @return    time since last cycle.
*/
double getDeltaTime()
{
    static double previousSeconds = 0.0f;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

    elapsedSeconds = (currentSeconds - previousSeconds) / 2.5;
    previousSeconds = currentSeconds;

    return elapsedSeconds; 
}

/**
*   Creates a 2d vector that contains the coordinate system of the tiles on the map.
* 
*  @return    2d vector.
*/
std::vector<std::vector<glm::vec2>> getMapCordinates()
{

    std::vector<std::vector<glm::vec2>> coordinates;
    const float pointsX = getMapTileSize()[0]+1;
    const float pointsY = getMapTileSize()[1]+1;

    coordinates.resize(pointsX, std::vector<glm::vec2>(pointsY));
   

    auto counter = 0;
    for (int i = 0; i < static_cast<int>(pointsY); i++) {
        for (int j = 0; j < static_cast<int>(pointsX); j++) {
            coordinates[j][i].x = ((j / (pointsX - 1)) * 2) - 1;
            coordinates[j][i].y = ((i / (pointsY - 1)) * 2) - 1;
        }
    }
    return coordinates;
}

/**
*   Gives every Ghost spawned a position within legal map limits.
*
*  @param     seed2 - parameter for random engine.
*  @see       getMapIndexData, getMapCoordiantes.
*/
glm::vec3 randomGhostPosition(int seed2)
{
    glm::vec3 spawnPosition;
    std::vector<std::vector<int>> mapIndexData = getMapIndexData();
    std::vector<std::vector<glm::vec2>> mapCordinates = getMapCordinates();

    std::vector<glm::vec2> validGhostPositions;

    for (int i = 0; i < mapIndexData.size(); i++) 
    {
        for (int j = 0; j < mapIndexData[i].size(); j++) 
        {
            if (mapIndexData[i][j] == 0) {
                glm::vec2 position = { i, j };
                validGhostPositions.push_back(position);
            }
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // creation of random seed.
    std::mt19937 engine(seed); // mersienne twister engine for randomness
    std::uniform_int_distribution<> dist(0, validGhostPositions.size()-1);

    auto random = dist(engine); // actual random number taken.

    int indexX = validGhostPositions[random].x; // not auto so intent is visible.
    int indexY = validGhostPositions[random].y;
    std::vector<float> offsets = getTileOffset();
    spawnPosition = { mapCordinates[indexX][indexY].x + offsets[0], mapCordinates[indexX][indexY].y + offsets[1], 0.0f};
    return spawnPosition;
}


/**
*   Creates fixed aspect ratio when fullscreen and gives resizable window functionality.
*
*  @param window - glfw window for resizing.
*  @param width  - recieves width of the window.
*  @param height - recieves height of the window.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    int a = 28;
    int b = 36;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // The "Graphics" width and height contain the desired pixel resolution to keep.
    glOrtho(0.0, a, b, 0.0, 1.0, -1.0);
    // Calculate the proper aspect ratio to use based on window ratio
    auto ratioX = width / (float)a;
    auto ratioY = height / (float)b;
    auto ratio = ratioX < ratioY ? ratioX : ratioY;
    // Calculate the width and height that the will be rendered to
    auto viewWidth = a * ratio;
    auto viewHeight = b * ratio;
    // Calculate the position
    auto viewX = ((width - a * ratio) / 2);
    auto viewY = ((height - b * ratio) / 2);
    // Apply the viewport and switch back to the GL_MODELVIEW matrix mode
    glViewport(viewX, viewY, viewWidth, viewHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

