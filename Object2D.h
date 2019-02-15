#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

typedef struct brick {
	float x;
	float y;
	float scale;
	std::string type;
};

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(std::string name, float radius, glm::vec3 color);

}

bool checkCollisionBrick(std::pair<float, float> ballPosition, float radius, float height, float width, brick brick);
bool checkCollisionSideWalls(std::pair<float, float> ballPosition, float radius);
bool checkCollisionTopBotWalls(std::pair<float, float> ballPosition, float radius, int botWall);
bool checkCollisionPlatform(std::pair<float, float> ballPosition, float radius, float height, float width, float platformX, float platformY);
bool checkCollisionBonus(std::pair<float, float> bonusPosition, float length, float height, float width, float platformX, float platformY);



