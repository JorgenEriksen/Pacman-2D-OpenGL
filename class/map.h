#pragma once

#include "GL/glew.h"
#include "Shader.h"


class Map
{
private:
	GLuint vao;
	GLuint ebo;
	GLuint squareShaderProgram;
	int tilesX;
	int tilesY;
	Shader shaderObject;
	GLuint shader;

public:
	Map();
	int getTilesX();
	int getTilesY();
	~Map();
	GLuint createMap();
	void drawMap();
	void deleteMap();


	
};
