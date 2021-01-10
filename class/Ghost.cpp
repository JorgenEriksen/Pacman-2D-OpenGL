/**
*   Ghost class that defines the functionality of ghost objects.
*	It shares elements of the pacman class.
*
*  @name Ghost.cpp
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "Ghost.h"
#include <time.h>
#include <random>
#include <chrono>

/**
*	Default constructor for ghost object.   
*
*  @param     pos - Position it recieves and applies.
*  @param     pos - Position that is sent down from mother class.
*  @see       setStartVelocity
*/
Ghost::Ghost(glm::vec3 pos) : MovingUnits(pos)
{
	Texture* textureobj = new Texture;
	auto textureobject = textureobj->create("../../../../assets/pacman.png");
	textureobject.apply(0);
	setStartVelocity();
}

/**
*   Destructor for ghost objects.
* 
*/
Ghost::~Ghost() {}

/**
*   Sets a velocity to begin with and checks it's surroundings for walls.
*
*  @see    isWall, changeVelocity
*/
void Ghost::setStartVelocity() {
	std::vector<glm::vec3> placesToMove;

	if (!MovingUnits::isWall(UP)) {
		placesToMove.push_back(UP);
	}
	if (!MovingUnits::isWall(DOWN)) {
		placesToMove.push_back(DOWN);
	}
	if (!MovingUnits::isWall(LEFT)) {
		placesToMove.push_back(LEFT);
	}
	if (!MovingUnits::isWall(RIGHT)) {
		placesToMove.push_back(RIGHT);
	}

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed);
	std::uniform_int_distribution<> dist(0, placesToMove.size() - 1);

	auto index = dist(engine);

	glm::vec3 startVelocity = placesToMove[index];

	changeVelocity(startVelocity);
}

/**
*   Checking for available paths that a ghost can take. L-shaped paths are favoured in this algorithm.
*
*  @see     isWall, getVelocity, newDirectionCommand, setNewVelocity.
*/
void Ghost::calculateMovement() {
	std::vector<glm::vec3> placesToMove;

	auto canMoveUp = !MovingUnits::isWall(UP);
	auto canMoveDown = !MovingUnits::isWall(DOWN);
	auto canMoveLeft = !MovingUnits::isWall(LEFT);
	auto canMoveRight = !MovingUnits::isWall(RIGHT);

	auto canChangeDirection = false;

	if (canMoveUp && (canMoveLeft || canMoveRight)) {
		canChangeDirection = true; // checking for L-shape
	}
	else if (canMoveDown && (canMoveLeft || canMoveRight)) {
		canChangeDirection = true;
	}
	
	if (canChangeDirection) {
		if (getVelocity() == UP && canMoveUp && canMoveDown) {
			canMoveDown = false;
		}
		else if (getVelocity() == DOWN && canMoveUp && canMoveDown) {
			canMoveUp = false;
		}
		else if (getVelocity() == LEFT && canMoveLeft && canMoveRight) {
			canMoveRight = false;
		}
		else if (getVelocity() == RIGHT && canMoveLeft && canMoveRight) {
			canMoveLeft = false;
		}

		if (canMoveUp)
			placesToMove.push_back(UP);
		if (canMoveDown)
			placesToMove.push_back(DOWN);
		if (canMoveLeft)
			placesToMove.push_back(LEFT);
		if (canMoveRight)
			placesToMove.push_back(RIGHT);



		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 engine(seed); // mersienne twister engine for good PRNG.
		std::uniform_int_distribution<> dist(0, placesToMove.size() - 1);

		auto index = dist(engine);

		glm::vec3 newVelocity = placesToMove[index];

		if (newDirectionCommand(newVelocity)) {
			setNewVelocity(newVelocity);
		}
	}
}