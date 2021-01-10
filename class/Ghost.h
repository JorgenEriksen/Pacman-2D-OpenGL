#pragma once

#include "MovingUnits.h"

class Ghost : public MovingUnits {
private:
	GLuint ghostVAO;
	GLuint ghostVBO;
	GLuint ghostShaderProgram;
public:
	Ghost(glm::vec3 pos);
	~Ghost();
	void setStartVelocity();
	std::vector<Ghost*> gGhost;
	void createGhost();
	void calculateMovement();
};
