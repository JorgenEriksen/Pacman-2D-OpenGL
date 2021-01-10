/**
*   Map class draws the map and displays it on the screen. It is made as one object of many vertices 
*   as opposed to many objects. This gives more control and allows for there to be a different background colour.
*
*  @name Map.cpp
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "map.h"
#include "../functions/utilityFunctions.h"
#include "../shaders/square.h"

/**
*   Creates/binds everything related to map objects.
*
*   @see   getMapTileSize, createMap.
*/

Map::Map()
{ 

	tilesX = getMapTileSize()[0];
	tilesY = getMapTileSize()[1];
	this->vao = createMap();
}

/**
*   Utility function for changing/getting dynamic size of map.
* 
*   @return   amount of tiles on x-axis
*/
int Map::getTilesX()
{
	return tilesX;
}

/**
*   Utility function for changing/getting dynamic size of map.
*
*   @return   amount of tiles on y-axis
*/
int Map::getTilesY()
{
	return tilesY;
}

/**
*   Placeholder destructor for map objects.
* 
*/
Map::~Map() {
}

/**
*   Creates map dynamically from data read from file. Uses vector to store all positions and indices. 
*   Draws map using indexed drawing.
* 
*  @return    vertex arrray object.
*  @see       getLevelVector
*/
GLuint Map::createMap()
{

	const auto pointsX = tilesX + 1.0f;
	const auto pointsY = tilesY + 1.0f;

	std::vector<GLfloat> map_positions;

	auto xCounter = 0;
	auto yCounter = 0;
	for (int i = 0; i < pointsX * pointsY; i++) {
		map_positions.push_back(((xCounter / (pointsX - 1)) * 2) - 1); // x axis
		map_positions.push_back(((yCounter / (pointsY - 1)) * 2) - 1); // y axis
		map_positions.push_back(0);				      // y axis
	
		xCounter++;
		if (xCounter > pointsX - 1) {  // new line
			xCounter = 0;
			yCounter++;
		}

	}
	
	std::vector<GLuint> indices;
	std::vector<int> levelData = getLevelVector();
	auto counter = 0;
	for (int i = 0; i < pointsX*tilesY; i++) { // dynamic size, allows for different maps.
		if (levelData[counter] == 1 && ((i + 1) % (tilesX+1) != 0)) {
			indices.push_back(0 + i);
			indices.push_back(1 + i);
			indices.push_back(tilesX + 1 + i);

			indices.push_back(1 + i);
			indices.push_back(tilesX + 1 + i);
			indices.push_back(tilesX + 2 + i);
		}
		counter++;
		if ((i + 1) % (tilesX + 1) == 0) {
			counter--;
		}
	}

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, map_positions.size() * sizeof(GLfloat), &map_positions[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (const void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	return vao;
}

/**
*   Diplays/draws the map on the screen. Gives the location to the uniform in the shader.
*
*   @see   createShader.
*/
void Map::drawMap()  {
	this->squareShaderProgram = shaderObject.createShader(squareVertexShaderSrc, squareFragmentShaderSrc);
	auto vertexColorLocation = glGetUniformLocation(squareShaderProgram, "u_Color");
	glUseProgram(squareShaderProgram);
	glBindVertexArray(vao);
	glUniform4f(vertexColorLocation, 0.0f, 0.0f, 1.0f, 1.0f); // color for the background.
	glDrawElements(GL_TRIANGLES, ((tilesX+1)* tilesY * 6), GL_UNSIGNED_INT, (const void*)0);
	
}

/**
*   Basic memory managment of map class.
* 
*  @see       cleanVAO
*/
void Map::deleteMap() {
	glDeleteProgram(squareShaderProgram);
	CleanVAO(vao);
}
