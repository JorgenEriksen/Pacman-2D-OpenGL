/**
*   Main file for our Pacman Game. This project is mainly a OpenGL project.
*	There are 3 external libraries that help support this application.
*   Currently there are four ghosts and one pacman with one single map.
*	It is possible to add more levels as that code is dynamic in that regard.
*
*  @name Pacman.exe
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "Pellets.h"
#include <algorithm>
#include <iterator>
#include "../shaders/pellets.h"

/**
*  Sets everything that is needed for pellet object, lastly it creates the object.
* 
*  @see  getTilesx, getTilesY, getLevelVector, CreatePellets.
*/
Pellets::Pellets(Map* map) {

	tilesX = map->getTilesX();
	tilesY = map->getTilesY();

	pelletsRemaining = 0;
	auto levelData = getLevelVector();
	for (int i = 0; i < levelData.size(); i++) {
		if (levelData[i] == 1) {
			pelletsAlive.push_back(1);
		}
		else if (levelData[i] != 2) {
			pelletsRemaining++;
		}
	}
	createPellets();
}

/**
*   Returns the vector with data about which pellet(s) are alive.
* 
*   @return  A vector with pellets that are alive/not alive.
*/
std::vector<int> Pellets::getPelletsAlive() {
	return pelletsAlive;
}

/**
*   For having the amount of pellets remaining as Pacman is moving.
* 
*   @return   Number of pellets remaning.
*/
int Pellets::getPelletsRemaining() {
	return pelletsRemaining;
}

/**
*   As pacman passes over, change the state of the pelletsAlive vector
*
*  @param     index - position in the array
*  @param     value - the relevant 0 or 1 that pacman passes.
*/
void Pellets::setPelletsAlive(int index, int value) {
	pelletsAlive[index] = value;

	if (value == 0)
		pelletsRemaining--;
	else 
		pelletsRemaining++;
}

/**
*   Utility function used for having the position of all pellets on the map.
* 
*  @return    Beskrivelse av evt. returverdi
*/
std::vector<glm::vec3> Pellets::getPelletsPositions() {
	return pelletsPositions;
}

/**
*   constructs all the pellets on the map as one object with many vertices.
* 
*  @see       getLevelVector
*/
void Pellets::createPellets() {

	const auto pointsX = tilesX + 1.0f;
	const auto pointsY = tilesY + 1.0f;

	std::vector<GLfloat> map_positions;

	auto xCounter = 0;
	auto yCounter = 0;
	for (int i = 0; i < pointsX * pointsY; i++) {
		map_positions.push_back(((xCounter / (pointsX - 1)) * 2) - 1); // x axis
		map_positions.push_back(((yCounter / (pointsY - 1)) * 2) - 1); // y axis
		map_positions.push_back(0);				                       // z axis


		xCounter++;
		if (xCounter > pointsX - 1) {  // new line
			xCounter = 0;
			yCounter++;
		}

	}

	std::vector<int> levelData = getLevelVector();
	std::vector<GLfloat> indices;

	auto counter = 0;
	for (int i = 0; i < tilesX * tilesY; i++) {
		if (i % tilesX == 0 && i != 0) { // If there is a newline in drawing, add the counter for next x-axis.
			counter++;
		}

		if (levelData[i] == 0) {
			indices.push_back(map_positions[counter * 3] + offsetX);
			indices.push_back(map_positions[(counter * 3) + 1] + offsetY);
			indices.push_back(0.0f);
			indices.push_back(1.0); // alive
		}
		counter++;
	};

	for (int i = 0; i < indices.size()/4; i++) { // dynamic size for pellets amount.
		glm::vec3 pos;
		pos.x = indices[i * 4];
		pos.y = indices[(i * 4) + 1];
		pos.z = 0.0f;
		pelletsPositions.push_back(pos);
	}

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, indices.size()*sizeof(GLfloat), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (const void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(3 * sizeof(float)));

	pelletVAO = vao;
	pelletVBO = vbo;
}

/**
*   Adding the shader and calling OpenGL to draw pellets on the screen.
*/
void Pellets::drawPellets() {
	this->pelletShaderProgram = shaderObject.createShader(pelletVertexShaderSrc, pelletFragmentShaderSrc);
	glUseProgram(pelletShaderProgram);
	glBindVertexArray(pelletVAO);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, pelletsPositions.size());
	glDisable(GL_PROGRAM_POINT_SIZE);
}

/**
*   updates a specific part of the vertex buffer with OpenGL function.
*   Works in part with the shader code to acheive disappearance.
*
*  @param     index - How much in the buffer to start changing data at.
*/
void Pellets::deletePellet(int index) {

	GLuint vertex_buffer;
	GLfloat data = 0.0f;
	auto offset = (index * 4) + 3; // the offset into the buffer where data will be replaced.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pelletVBO);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*offset, sizeof(GLfloat), &data);
}