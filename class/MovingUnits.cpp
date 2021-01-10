/**
*   Subclass of Gameobject and motherclass of Pacman and Ghost.
*	This class provides most of the functionality of the game physics/logic wise.
*	MovingUnits is powerful and can be understood as the engine of the game.
* 
*  @name MovingUnits.cpp
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "MovingUnits.h"
#include <iostream>

/**
*   Constructor for Pacman. 
*
*  @param     pos - gamobject position
*  @param     vel - gamobject velocity
*
*  @see          ChangeVelocity, init, createObject.
*/
MovingUnits::MovingUnits(glm::vec3 pos, glm::vec3 vel) : GameObject(pos) {
	type = MovingUnitType::PACMAN;
	changeVelocity(vel);			// change velocity
	init();						    // sets shader, newVelocity, animationTime and animationAdder
	createObject();					// sets start texture
	yOffset = 0.0f;				    // uses in texture
}

/**
*   Constructor for Ghost
*
*  @param     pos - gamobject position
*  @param     vel - gamobject velocity

*  @see          Init, createObject
*/
MovingUnits::MovingUnits(glm::vec3 pos) : GameObject(pos) {
	type = MovingUnitType::GHOST;
	init();							// sets shader, newVelocity, animationTime and animationAdder
	createObject();					// sets start texture
}

/**
*   Performing clean up for memory of objects.
*  
*/
MovingUnits::~MovingUnits() {}

/**
*   sets shader, newVelocity, animationTime and animationAdder
*/
void MovingUnits::init() {
	shader = shaderObject.shaderFile("../../../../shaders/movingUnit.shader");
	shaderObject.bind();
	shaderObject.setUniform1i("u_Texture", 0);
	newVelocity = NONE;
	animationTime = 0.0f;
	animationTimeAdder = 1;
}

/**
*   creates animation by chaning texture related to delta time
* 
*   @param     dt - delta time
*/
void MovingUnits::animation(double dt) {

	float xOffset = 0.0f;
	float xOffset2 = 0.0f;
	float offset = 0.02f;		// offset from space difference between pacman and ghost in texture image (pacman.png)

	float animationSpeed = 35; // the higher, the faster the animation will be.

	animationTime += dt * animationSpeed * animationTimeAdder;

	if (type == MovingUnitType::PACMAN) {
		if (animationTime > 4)  // Starts cycle counting down.
		{
			animationTime = 3;
			animationTimeAdder = -1;
		}
		else if (animationTime < 0)  // This code is for the correct repeating of texture cyclces. Next cycle is counting up
		{							 
			animationTime = 1;
			animationTimeAdder = 1;
		}
		xOffset = 0.16f * floor(animationTime);
		xOffset2 = 0.16f;
	}
	else 
	{ // type == MovingUnitType::GHOST
		if (animationTime > 2) 
		{
			animationTime = 0;
		}
		
		xOffset = 0.16f * 4 + offset; // offset is needed for last gap in texture file between pacman and ghost.
		xOffset += floor(animationTime) * 0.16f;
		xOffset2 = 0.17999f;
	}

	if (velocity == UP)
		yOffset = 0.50f;
	else if (velocity == DOWN)
		yOffset = 0.75f;
	else if (velocity == LEFT)
		yOffset = 0.25f;
	else if (velocity == RIGHT)
		yOffset = 0.0f;

	std::vector<glm::vec2> uvAnim;

	if (velocity == STOP)
	{
		if (type == MovingUnitType::PACMAN) // if pacman is standing still, have the mouth open.
		{
			uvAnim =
			{
				glm::vec2(0.16f, yOffset        ),	// 0
				glm::vec2(0.32f, yOffset        ),	// 1
				glm::vec2(0.32f, yOffset + 0.25f),	// 2
				glm::vec2(0.16f, yOffset + 0.25f)	// 3
			};
		} else 
		{ 
			uvAnim =
			{
				glm::vec2(0.16f * 4 + offset,	       yOffset         ),	// 0
				glm::vec2(0.16f * 4 + offset + 0.179f, yOffset        ),	// 1
				glm::vec2(0.16f * 4 + offset + 0.179f, yOffset + 0.25f),	// 2
				glm::vec2(0.16f * 4 + offset,	       yOffset  + 0.25f )	// 3
			};
		}
	} else // if velocity is not STOP
	{ 
		uvAnim =
		{
			glm::vec2(xOffset,		     yOffset      ),	// 0
			glm::vec2(xOffset+ xOffset2, yOffset      ),	// 1
			glm::vec2(xOffset+ xOffset2, yOffset+0.25f),	// 2
			glm::vec2(xOffset,		     yOffset+0.25f)	// 3
		};
	}

	glBindBuffer(GL_ARRAY_BUFFER, UVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &uvAnim[0], GL_DYNAMIC_DRAW);
}

/**
*   Creates and stores the correct texture cords and object based on input from constructor.
*  
*/
void MovingUnits::createObject() {

	std::vector<glm::vec2> objectVec;   
	std::vector<glm::vec2> texCoordsVec; // store the texture coordinates in vec2

	if (type == MovingUnitType::PACMAN) {// create the pacman object with vec2
		
		objectVec =
		{
			glm::vec2(-0.027f,-0.027f),	// 0
			glm::vec2(0.027f,-0.027f),  // 1
			glm::vec2(0.027f, 0.027f),	// 2
			glm::vec2(-0.027f, 0.027f)	// 3
		};

		
		texCoordsVec = // store the texture coordinates in vec2
		{
			glm::vec2(0.0f, 0.0f),	// 0
			glm::vec2(1.0f, 0.0f),  // 1
			glm::vec2(1.0f, 1.0f),	// 2
			glm::vec2(0.0f, 1.0f)	// 3
		};
	}

	else { // type == MovingUnitType::GHOST

		objectVec =
		{
			glm::vec2(-0.027f,-0.027f),	// 0
			glm::vec2(0.027f,-0.027f),  // 1
			glm::vec2(0.027f, 0.027f),	// 2
			glm::vec2(-0.027f, 0.027f)	// 3
		};

		// store the texture coordinates in vec2
		texCoordsVec =
		{
			glm::vec2(0.86f, 0.0f),	// 0
			glm::vec2(1.00f, 0.0f),  // 1
			glm::vec2(1.00f, 0.25f),	// 2
			glm::vec2(0.86f, 0.25f)	// 3
		};

	} 

	// indices to create the square
	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// create the vertex array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// for vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &objectVec[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// for uv buffer
	glGenBuffers(1, &UVbo);
	glBindBuffer(GL_ARRAY_BUFFER, UVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &texCoordsVec[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// for index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

/**
*   Draws the correct moving unit on the screen.
* 
*   @see         Transform
*/
void MovingUnits::draw() {
	shaderObject.bind();
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	Transform(shader);
}

/**
*   
*  Gives data to the shaders so the vertices can be adjusted accordingly.
* 
*  @param     shaderprogram - retrieves the correct compiled progra
*/
void MovingUnits::Transform(const GLuint shaderprogram)
{
	glm::mat4 model; // using model matrix to translate, this is multiplied in the shader.
	model = glm::translate(glm::mat4(1.0f), getPosition());
	GLuint transformationmat = glGetUniformLocation(shaderprogram, "model");
	glUniformMatrix4fv(transformationmat, 1, GL_FALSE, glm::value_ptr(model));
}

/**
*   Getter for velocity of a movingunit object.
* 
*   @return  velocity.
* 
*/
glm::vec3 MovingUnits::getVelocity()
{
	return velocity;
}

/**
*   Changing the velocity using class member.
*
*   @param   vel - changes the direction of object.
*/
void MovingUnits::changeVelocity(glm::vec3 vel)
{
	velocity = vel;
}

/**
*   Takes a new velocity and adds it into a waiting state to be executed.
*
*   @param   vel - adds velocity into the que (of velocities) as the next to be performed.
*/
void MovingUnits::setNewVelocity(glm::vec3 vel)
{
	newVelocity = vel;
}

/**
*   Handles position and checking collision with walls.
* 
*   @param     dt - used to keep program in time with correct delta time.
*   @see       getPosition, getMapPositionIndex, checkWallCollision, changeVelocity, setPosition
*/
void MovingUnits::moveObject(float dt) {
	auto speed = 0.5f; // The variable for changing Pacman's movement speed.
	
	animation(dt); // runs the animation of all objects.
	glm::vec3 position = getPosition();
	position = position + (velocity * dt * speed);

	glm::vec2 unitMapIndex = getMapPositionIndex();
	auto corX = unitMapIndex.x;
	auto corY = unitMapIndex.y;

	// checking for edge case at map exit(s). 
	if (position.x <= -0.99f && velocity == LEFT) 
	{
		position.x = 0.999f;
	}
	else if (position.x >= 0.99f && velocity == RIGHT) 
	{
		position.x = -0.999f;
	}

	checkWallCollision(velocity);

	if (velocity == STOP) {
		changeVelocity(newVelocity);
		newVelocity = NONE;
	}
	else if (newVelocity != NONE) {
		if (position.x >= gCoordinates[prevTurnPointX][prevTurnPointY].x + offsetX && velocity == RIGHT) { // if you can turn right
			if (newVelocity == UP) { // if your direction is up.
				changeVelocity(UP);
				newVelocity = NONE;
				position.x = gCoordinates[prevTurnPointX][prevTurnPointY].x + offsetX;
			}
			if (newVelocity == DOWN) {
				changeVelocity(DOWN);
				newVelocity = NONE;
				position.x = gCoordinates[prevTurnPointX][prevTurnPointY].x + offsetX;
			}
		}

		if (position.x <= gCoordinates[prevTurnPointX][prevTurnPointY].x + offsetX && velocity == LEFT) {
			if (newVelocity == UP) {
				changeVelocity(UP);
				newVelocity = NONE;
				position.x = gCoordinates[prevTurnPointX][prevTurnPointY].x + offsetX;
			}
			else if (newVelocity == DOWN) {
				changeVelocity(DOWN);
				newVelocity = NONE;
				position.x = gCoordinates[prevTurnPointX][prevTurnPointY].x + offsetX;
			}
		}

		if (position.y >= gCoordinates[prevTurnPointX][prevTurnPointY].y + offsetY && velocity == UP) {
			if (newVelocity == RIGHT) {
				changeVelocity(RIGHT);
				newVelocity = NONE;
				position.y = gCoordinates[prevTurnPointX][prevTurnPointY].y + offsetY;
			}
			else if (newVelocity == LEFT) {
				changeVelocity(LEFT);
				newVelocity = NONE;
				position.y = gCoordinates[prevTurnPointX][prevTurnPointY].y + offsetY;
			}
		}

		if (position.y <= gCoordinates[prevTurnPointX][prevTurnPointY].y + offsetY && velocity == DOWN) {
			if (newVelocity == RIGHT) {
				changeVelocity(RIGHT);
				newVelocity = NONE;
				position.y = gCoordinates[prevTurnPointX][prevTurnPointY].y + offsetY;
			}
			else if (newVelocity == LEFT) {
				changeVelocity(LEFT);
				newVelocity = NONE;
				position.y = gCoordinates[prevTurnPointX][prevTurnPointY].y + offsetY;
			}
		}

	}
	setPosition(position);
}


/**
*   Ruleset for checking wall collision around the map.
*   Checks every wall around and ahead of him.
* 
*  @param     Beskrivelse av 1.parameter
*  @return    If collision has occured the function returns true.
*  @see       getPosition, getMapPositionIndex, changeVelocity, isWall.
*/
bool MovingUnits::checkWallCollision(glm::vec3 direction) {
	glm::vec3 unitPosition = getPosition();
	glm::vec2 unitMapIndex = getMapPositionIndex();
	
	auto corX = unitMapIndex.x;
	auto corY = unitMapIndex.y;



	if (direction == UP && isWall(direction)) {
		if (unitPosition.y >= gCoordinates[corX][corY + 1].y - offsetY) {
			changeVelocity(STOP);
			return false;
		}

		return true;
	} else if (direction == DOWN && isWall(direction)) {
		if (unitPosition.y <= gCoordinates[corX][corY].y + offsetY) {
			changeVelocity(STOP);
			return false;
		}

		return true;
	} else if (direction == LEFT && isWall(direction)) {

		if (unitPosition.x <= gCoordinates[corX][corY].x + offsetX) {
			changeVelocity(STOP);
			return false;
		}
	} else if (direction == RIGHT && isWall(direction)) {
		
		if (unitPosition.x >= gCoordinates[corX + 1][corY].x - offsetX) {
			changeVelocity(STOP);
			return false;
		}
	}
	return true;
}

/**
*   Checks if the next box ahead of themselves is a wall or not.
* 
*
*  @param     direction - The direction that pacman or ghost are pointing in.
*  @return    returns true if box ahead of them is a wall.
*  @see       getPosition, getMapPositionIndex.
*/
bool MovingUnits::isWall(glm::vec3 direction) {

	glm::vec3 unitPosition = getPosition();

	glm::vec2 unitMapIndex = getMapPositionIndex();
	auto unitX = unitMapIndex.x;
	auto unitY = unitMapIndex.y;

	if (direction == UP && gMapIndexData[unitX][unitY + 1] == 1) {
		return true;
	}
	else if (direction == DOWN && gMapIndexData[unitX][unitY - 1] == 1) {
		return true;
	}
	else if (direction == LEFT && unitX >= 1) {
		if (gMapIndexData[unitX - 1][unitY] == 1) {
			return true;
		}
	}
	else if (direction == RIGHT && unitX <= 26) {
		if (gMapIndexData[unitX + 1][unitY] == 1) {
			return true;
		}
	}
	return false;
}

/**
*   Changes the current direction a pacman/ghost if facing. Act similarly to a queue.
* 
*
*  @param     direction - the new direction the gameobject will be pointing in.
*  @return    If you can not move where you attempted to move, return false.
*  @see       isWall, getMapPositionIndex.
*/
bool MovingUnits::newDirectionCommand(glm::vec3 direction) {

	// if new direction is not the same as current velocity
	if (direction != velocity && !isWall(direction) == true) {
		glm::vec2 unitMapIndex = getMapPositionIndex();
		auto unitX = unitMapIndex.x;
		auto unitY = unitMapIndex.y;
		if (unitX != prevTurnPointX || unitY != prevTurnPointY) {
			prevTurnPointX = unitMapIndex.x;
			prevTurnPointY = unitMapIndex.y;
			return true;
		} 
		return false;
	};
	return false;
}


