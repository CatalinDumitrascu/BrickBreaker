#include "Object2D.h"

#include <Core/Engine.h>


Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(height, 0, 0), color),
		VertexFormat(corner + glm::vec3(height, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateCircle(std::string name, float radius, glm::vec3 color)
{	

	const float PI = 3.1415926f;

	std::vector<VertexFormat> result;

	float increment = 2.0f * PI / 10000;

	for (float currAngle = 0.0f; currAngle <= 2.0f * PI; currAngle += increment)
	{
		result.push_back(VertexFormat(glm::vec3(radius * cos(currAngle) + 0, radius * sin(currAngle) + 0, 0), color));
	}


	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i < result.size(); i++) {
		indices.push_back(i);
	}

	square->SetDrawMode(GL_TRIANGLE_FAN);
	
	square->InitFromData(result, indices);
	
	return square;
}

int a = 0;
bool checkCollisionBrick(std::pair<float, float> ballPosition, float radius, float height, float width, brick brick) //Funcfion for checking collision
{   
	if (ballPosition.second + radius < brick.y) return false; //ball is lower
	else if (ballPosition.second - radius > brick.y + height) return false; //ball is higher
	else if (ballPosition.first + radius < brick.x) return false; //ball is lefter
	else if (ballPosition.first - radius > brick.x + width) return false; //ball is righter
	return true; //There is a collision because none of above returned false
}

bool checkCollisionPlatform(std::pair<float, float> ballPosition, float radius, float height, float width, float platformX, float platformY) //Funcfion for checking collision
{	
	if (ballPosition.first + radius > platformX - width/2 && ballPosition.first - radius < platformX + width/2) {
		if (ballPosition.second - radius < platformY + height) {
			return true;
		}
	}
	return false;
}

bool checkCollisionSideWalls(std::pair<float, float> ballPosition, float radius) //Funcfion for checking collision
{
	if (ballPosition.first + radius < 1270 && ballPosition.first - radius > 10) return false; //ball is lefter
	
	return true; //There is a collision because none of above returned false
}

bool checkCollisionTopBotWalls(std::pair<float, float> ballPosition, float radius, int botWall) 
{
	if (botWall > 0) {
		if (ballPosition.second + radius < 710 && ballPosition.second - radius > 10) return false; //ball is lower
	}
	else if (ballPosition.second + radius < 710) return false;
	return true;
}

bool checkCollisionBonus(std::pair<float, float> bonusPosition, float length, float height, float width, float platformX, float platformY)
{
	if (bonusPosition.first + length > platformX - width / 2 && bonusPosition.first < platformX + width / 2) {
		if (bonusPosition.second < platformY + height) {
			return true;
		}
	}
	return false;
}