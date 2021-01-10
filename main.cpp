/**
*   Main file for our Pacman Game. This project is mainly a OpenGL project. 
*	There are 3 external libraries that help support this application. 
*   Currently there are four ghosts and one pacman with one single map.
*	It is possible to add more levels as that code is dynamic in that regard.
* 
*   @name Pacman.exe
*   @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "class/Pacman.h"
#include "class/MovingUnits.h"
#include "class/Pellets.h"
#include "functions/utilityFunctions.h"
#include "functions/pacmanStartingPos.h"


GLFWwindow* gWindow = NULL;

bool init();
void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

glm::vec3 pacmanStartingVel(-1.0f, 0.0f, 0.0f);	// we have decided not to set a random starting (as in ghost) on pacman, 
												// as he spawns right next to the edge of the map

auto numberOfGhosts = 4;						// easily accessible for changing amount of ghost

int main() {
	// Initializations and debug output
	if (!init()) {
		return EXIT_FAILURE;
	}

	Map* map = new Map;

	Pellets* pellets = new Pellets(map);
	
	std::vector<Ghost*> ghosts;
	for (int i = 0; i < numberOfGhosts; i++) {
		glm::vec3 ghostStartingPos = randomGhostPosition(i);
		Ghost* ghost = new Ghost(ghostStartingPos);
		ghosts.push_back(ghost);
	}

	glm::vec3 pacmanPosition = getPacmanStartingPosition();
	auto pacman = std::make_unique<Pacman>(pacmanPosition, pacmanStartingVel);

	while (!glfwWindowShouldClose(gWindow)) 
	{
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		double dt = getDeltaTime();

		map->drawMap();
		pellets->drawPellets();

		pacman->moveObject(dt);
		pacman->draw();
		pacman->userInput(gWindow, dt);
		pacman->checkCollision(pellets);
		pacman->checkCollision(ghosts);

		for (int i = 0; i < numberOfGhosts; i++) {
			ghosts[i]->moveObject(dt);
			ghosts[i]->draw();
			ghosts[i]->calculateMovement();
		}

		// if player won the game
		if (pellets->getPelletsRemaining() == 0)
		{	
			// freeze all moving units
			pacman->changeVelocity(STOP);
			for (int i = 0; i < ghosts.size(); i++) {
				ghosts[i]->changeVelocity(STOP);
			}
		}
		glfwSwapBuffers(gWindow); // swap front and back buffers (drawing one buffer while showing the other)
	}
	
	delete map;
	delete pellets;
	for (int i = 0; i < ghosts.size(); i++) { // manual memory managment.
		delete ghosts[i];                     // Smart pointers did not work, therefore manual managment.
		ghosts.clear();
	}

	glfwTerminate();
	return 0;
}

bool init() {
	glewExperimental = GL_TRUE;

	if (!glfwInit()) {
		std::cerr << "GLFW init failed" << std::endl;
		return false;
		std::cin.get();

	}

	/* Configure window */
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4); // telling opengl to use different type of buffer.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gWindow = glfwCreateWindow(840, 1080, "Pacman", NULL, NULL);
	if (gWindow == NULL) {
		std::cerr << "Window init failed" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);
	glfwSetFramebufferSizeCallback(gWindow, framebuffer_size_callback);


	GLenum error = glewInit();

	if (error != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND); // enabling blend for pellets
	glEnable(GL_MULTISAMPLE); // enabling multi-sampling anti-aliasing.
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	return true;
}

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {
	std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
		"type = 0x" << type <<
		", severity = 0x" << severity <<
		", message =" << message << "\n";
};










