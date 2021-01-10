#pragma once

#include "../global/global.h"
#include "Texture.h"

class GameObject {
private:
	glm::vec3 position;

	public:
		GameObject(glm::vec3 pos);
		glm::vec3 getPosition();
		void setPosition(glm::vec3 pos);
		glm::vec2 getMapPositionIndex();
};


