#include "pacmanStartingPos.h";

/*
*
*   This function can't be in utility functions becuase the variables then have to be gotten with functions.
*   That causes the program to lag and slow down during start-up which makes the ghosts and pacman move in walls.
*   
*   
*/


/**
*   Creates the final shader to be attached to the objects.
*
*/
glm::vec3 getPacmanStartingPosition() {
    std::vector<int> tiles = getMapTileSize();
    int tilesX = tiles[0];
    int tilesY = tiles[1];
    int spawnX = 0;
    int spawnY = 0;

    for (int i = 0; i < tilesY; i++) {
        for (int j = 0; j < tilesX; j++) {
            if (gMapIndexData[j][i] == 2) {
                spawnX = j; spawnY = i;
            }
        }
    }
    glm::vec2 posPlaceholder = gCoordinates[spawnX][spawnY];
    glm::vec3 pacmanPosition = { posPlaceholder.x + (offsetX*(2/3)), posPlaceholder.y + offsetY, 0.0f };
    return pacmanPosition;
}