/**
*   Main file for our Pacman Game. This project is mainly a OpenGL project.
*	There are 3 external libraries that help support this application.
*   Currently there are four ghosts and one pacman with one single map.
*	It is possible to add more levels as that code is dynamic in that regard.
*
*  @name Pacman.exe
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/


#include <math.h>
#include "Pacman.h"


/**
*   Default pacman creation.
* 
*  @param     pos - Pacman's position
*  @param     vel - Pacman's velocity
*  @param     pos, vel - recievers for pacman's data.
*/
Pacman::Pacman(glm::vec3 pos, glm::vec3 vel) : MovingUnits(pos, vel)
{
	Texture* textureobj = new Texture;
	auto textureobject = textureobj->create("../../../../assets/pacman.png");
	textureobject.apply(0);

}

/**
*   Destructor.
* 
*/
Pacman::~Pacman()
{

}

/**
*   En kort en-setnings beskrivelse av funksjonen. Avsluttes med ‘.’ (punktum).
*      (frivillig å ha med blank linje eller ei)
* 
*
*  @param     window - window that the userinput acts upon.
*  @param     dt - used to keep things in time according to deltatime.
*  @see       newDirectionCommand, setNewVelocity.
*/
void Pacman::userInput(GLFWwindow* window, double dt)
{
	// UP
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		if (newDirectionCommand(UP)) 
		{
			setNewVelocity(UP);
		}
	}

	// LEFT
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		if (newDirectionCommand(LEFT)) 
		{
			setNewVelocity(LEFT);
		}
	}

	// DOWN
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		if (newDirectionCommand(DOWN)) 
		{
			setNewVelocity(DOWN);
		}
	}

	// RIGHT
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		if (newDirectionCommand(RIGHT)) 
		{
			setNewVelocity(RIGHT);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwTerminate();
	}
}

/**
*   Checks pacman's collision against ghosts and freezing game mechanics.
*
*  @param    ghosts - Ghost objects that pacman will be testing collision against.
*  @see      getPosition, changeVelocity
*/
void Pacman::checkCollision(std::vector<Ghost*> ghosts)
{
	glm::vec3 pacmanPos = getPosition();
	bool collision = false;

	for (int i = 0; i < ghosts.size(); i++) {
		glm::vec3 ghostPos = ghosts[i]->getPosition();
		if (distance(pacmanPos, ghostPos) < 0.05f) {
			collision = true;
		}
	}
	
	if (collision) {
		changeVelocity(STOP);
		for (int i = 0; i < ghosts.size(); i++) {
			ghosts[i]->changeVelocity(STOP);
		}
	}


}

/**
*   Checking for collision against pellet object.
* 
*
*  @param     pellets - the pellet object pacman checks collision against.
*  @see       getPosition, getPelletsAlive, getPelletsPosition, deletePellet, setPelletsAlive.
*/
void Pacman::checkCollision(Pellets* pellets) {
	glm::vec3 pacmanPos = getPosition();
	std::vector<int> pelletsAlive = pellets->getPelletsAlive();
	std::vector<glm::vec3> pelletsPos = pellets->getPelletsPositions();

	// bestemmer meg her at jeg er usikker om for loop int skal være auto, mest sannsynlig JA! jippi
	for (int i = 0; i < pelletsPos.size(); i++) {
		if (distance(pacmanPos, pelletsPos[i]) < 0.03f) {	// if in eat distance
			if (pelletsAlive[i] == 1)						// if pellet is not eaten
			{
				pellets->deletePellet(i);
				pellets->setPelletsAlive(i, 0);
			}
		}
	}

}

/**
*  Mathemathical apporach that checks the distance between two vectors. A type of collision check.
*
*  @param     vector - initial vector
*  @param     vector2 - vector that distance is compared against.
*  @return    Beskrivelse av evt. returverdi
*/
float distance(glm::vec3 vector, glm::vec3 vector2) 
{
	return sqrt(pow(vector.x - vector2.x, 2) + pow(vector.y - vector2.y, 2));
}



