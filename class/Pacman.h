#pragma once

#include "Ghost.h"
#include "Pellets.h"
//#include "Texture.h"


class Pacman : public MovingUnits
{
private:

	/*Pacman_struct pacman;
	Texture_struct texture;*/
	/*Sprite* sp;
	AnimationType current_animation;*/

	//Sprite* sp;

	//AnimationType current_animation;

public:

	Pacman(glm::vec3 pos, glm::vec3 vel);
	~Pacman();


	void userInput(GLFWwindow* window, double dt);
	void checkCollision(std::vector<Ghost*> ghosts);
	void checkCollision(Pellets* pellets);
};
