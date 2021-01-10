#pragma once
/*
* 
*  File containing global variables used in the program. These have uses all over the program and are therefore global.
*  Stops unncessecery heavy calculations by making variable constant from function,
*  and use it multiple times, instead of running fuctions multiple times.
* 
*  @name global.h
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "../functions/utilityFunctions.h"



static std::vector<int> gLevelData = getLevelVector();
static std::vector<std::vector<glm::vec2>> gCoordinates = getMapCordinates();
static std::vector<std::vector<int>> gMapIndexData = getMapIndexData();

static const float offsetX = getTileOffset()[0];
static const float offsetY = getTileOffset()[1];

const glm::vec3 UP(0.0f, 1.0f, 0.0f);
const glm::vec3 DOWN(0.0f, -1.0f, 0.0f);
const glm::vec3 LEFT(-1.0f, 0.0f, 0.0f); // global movement vectors
const glm::vec3 RIGHT(1.0f, 0.0f, 0.0f);
const glm::vec3 STOP(0.0f, 0.0f, 0.0f);
const glm::vec3 NONE(0.0f, 0.0f, 0.0f);

enum class MovingUnitType // type that gets sent into object creation stage.
{
	PACMAN,
	GHOST
};