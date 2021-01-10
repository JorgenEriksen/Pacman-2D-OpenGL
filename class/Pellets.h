#pragma once
#include "GameObject.h"


class Pellets {

private:
	GLuint pelletVAO;
	GLuint pelletVBO;
	GLuint pelletShaderProgram;
	std::vector<int> pelletsAlive;
	std::vector<glm::vec3> pelletsPositions;
	int pelletsRemaining;

	int tilesX;
	int tilesY;

	Shader shaderObject;
	GLuint shader;

public:
	Pellets(Map* map);
	std::vector<int> getPelletsAlive();
	void setPelletsAlive(int index, int value);
	std::vector<glm::vec3> getPelletsPositions();
	int getPelletsRemaining();
	void createPellets();
	void deletePellet(int index);
	void drawPellets();


};

