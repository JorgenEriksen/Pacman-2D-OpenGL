#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "GameObject.h"

#include "Shader.h"




class MovingUnits : public GameObject {
private:
	glm::vec3 velocity;
	glm::vec3 newVelocity;
	float angle;
	int prevTurnPointX;
	int prevTurnPointY;
	double animationTime;
	int animationTimeAdder;

	float yOffset = 0.0f; 


	MovingUnitType type;

	Shader shaderObject;
	GLuint shader;

	GLuint vao;	// vertex array object
	GLuint vbo;	// vertex buffer object
	GLuint ibo;	// index buffer object
	GLuint cbo;	// color buffer object
	GLuint UVbo;	// uv buffer object

public:

	MovingUnits(glm::vec3 pos, glm::vec3 vel);
	MovingUnits(glm::vec3 pos);
	~MovingUnits();
	void init();
	void animation(double dt);
	void createObject();
	void draw();
	void Transform(const GLuint shaderprogram);
	glm::vec3 getVelocity();
	void changeVelocity(glm::vec3 vel);
	void setNewVelocity(glm::vec3 vel);
	void moveObject(float dt);
	bool checkWallCollision(glm::vec3 direction);
	bool isWall(glm::vec3 direction);
	bool newDirectionCommand(glm::vec3 direction);

};

