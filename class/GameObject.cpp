/**
*   Gameobject class that gives position to it's subclasses. 
*   It has utility functions that are used throught the entire application.
*   It serves as the motherclass for moving units class and grandmother for pacman and ghost.
*
*  @name GameObject.cpp
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "GameObject.h"
/**
*   Constructer that sets the gameobject position.
*
*  @param     pos - gameobjects init position
*/
GameObject::GameObject(glm::vec3 pos)
{
	position = pos; 
}

/**
*  Getter for position.
* 
*  @return    gameobject position
*/
glm::vec3 GameObject::getPosition() {

	return position;
}

/**
*   Setter for gameobject position
*/
void GameObject::setPosition(glm::vec3 pos) {
	position = pos;
}


/**
*   gets the index for map position.
* 
*  @return    x and y index to map position index
*/
glm::vec2 GameObject::getMapPositionIndex() {
	int tilesX = getMapTileSize()[0];
	int tilesY = getMapTileSize()[1];

	float unitX = (((position.x + 1) / 2) * tilesX) + 1;
	float unitY = (((position.y + 1) / 2) * tilesY) + 1;

	int corX = floor(unitX) - 1;
	int corY = floor(unitY) - 1;

	glm::vec2 posIndex = { corX, corY };
	return posIndex;
}


