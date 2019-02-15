#include "Brickbreaker.h"

#include <vector>
#include <iostream>
#include <map>
#include <utility>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"


using namespace std;


Brickbreaker::Brickbreaker()
{
}

Brickbreaker::~Brickbreaker()
{
}

std::pair <float, float> ballPosition;
std::pair <float, float> ballMove;
std::pair <float, float> bonusPosition;
std::vector <brick> bricks;
std::vector <brick> anime;

float platformHeight = 20;
float platformWidth = platformHeight * 10;

float brHeight = 20;
float brWidth = 56;

float sideHeight = 720;
float sideWidth = 10;

float topHeight = 10;
float topWidth = 1280;

float ballRadius = 12;

int forStart = 0;
int yesBonus = 0;
int bonus;
int botWall = 0;
int nrLives = 3;

void Brickbreaker::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	
	
	Mesh* brRed = Object2D::CreateSquare("brRed", corner, brHeight, brWidth, glm::vec3(1, 0, 0), true);
	Mesh* brYellow = Object2D::CreateSquare("brYellow", corner, brHeight, brWidth, glm::vec3(1, 1, 0), true);
	Mesh* brBlue = Object2D::CreateSquare("brBlue", corner, brHeight, brWidth, glm::vec3(0, 0, 1), true);
	AddMeshToList(brRed);
	AddMeshToList(brYellow);
	AddMeshToList(brBlue);

	Mesh* sideWall = Object2D::CreateSquare("sideWall", corner, sideHeight, sideWidth, glm::vec3(0.7f, 0.6f, 0.4f), true);
	AddMeshToList(sideWall);

	Mesh* topWall = Object2D::CreateSquare("topWall", corner, topHeight, topWidth, glm::vec3(0.7f, 0.6f, 0.4f), true);
	AddMeshToList(topWall);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, platformHeight, platformWidth, glm::vec3(0, 0, 1));
	AddMeshToList(square3);

	Mesh* bonus = Object2D::CreateSquare("bonus", corner, 20, 20, glm::vec3(0, 1, 1));
	AddMeshToList(bonus);

	Mesh* ball = Object2D::CreateCircle("ball", ballRadius, glm::vec3(1, 0, 0));
	AddMeshToList(ball);

	Mesh* life = Object2D::CreateCircle("life", ballRadius/2 + 2, glm::vec3(1, 0, 1));
	AddMeshToList(life);

	ballPosition = std::make_pair(640, 80);
	ballMove = std::make_pair(1, 1);


	brick aux;
	float posx = 30;
	float posy = 360;

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 9; j++) {
			if (i < 5) {
				aux.x = posx + 12;
				aux.y = posy + 5;
				aux.type = "brRed";
				aux.scale = 1.0f;
				bricks.push_back(aux);
				posy += 35;
			}
			else if (i >= 5 && i < 10) {
				aux.x = posx + 12;
				aux.y = posy + 5;
				aux.type = "brYellow";
				aux.scale = 1.0f;
				bricks.push_back(aux);
				posy += 35;
			}
			else {
				aux.x = posx + 12;
				aux.y = posy + 5;
				aux.type = "brBlue";
				aux.scale = 1.0f;
				bricks.push_back(aux);
				posy += 35;
			}
		}
		posy = 360;
		posx += 80;
	}
}

void Brickbreaker::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

float rotateangle = 0;
float posx = 150, posy = 250;
float trspeed = 2;

float speed = 300.5f;
int mouse = 0;
int platformaX;
int game = 0;


void Brickbreaker::Update(float deltaTimeSeconds)
{
	glm::ivec2 cursorPosition = window->GetCursorPosition();
	platformaX = cursorPosition.x;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformaX - 100, 30); //platforma
	RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
	
	for (auto aux : bricks) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(aux.x, aux.y); 
		RenderMesh2D(meshes[aux.type], shaders["VertexColor"], modelMatrix);
	}
	int lifex = 20, lifey = 20;

	for (int i = 0; i < nrLives; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(lifex, lifey);
		RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
		lifex += 18;
	}

	if (ballPosition.second < 2) {
		ballPosition.second = 80;
		ballMove.second = -ballMove.second;
		nrLives--;
		game = 0;
		mouse = 0;
		speed = 300.5f;
	}

	if (nrLives == 0) {
		game = 2;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 0);
	RenderMesh2D(meshes["sideWall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(1270, 0);
	RenderMesh2D(meshes["sideWall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 710);
	RenderMesh2D(meshes["topWall"], shaders["VertexColor"], modelMatrix);
	glm::mat3 aux;
	
	if (mouse >= 1) ballPosition.second += (deltaTimeSeconds * speed * ballMove.second);
	int it = 0;
	bool collision = false;
	
	for (auto aux : bricks) {
		if (checkCollisionBrick(ballPosition, ballRadius, brHeight, brWidth, aux)) {
			ballMove.second = -ballMove.second;
			collision = true;
			anime.push_back(aux);
			if (yesBonus == 0) {
				bonusPosition.first = aux.x;
				bonusPosition.second = aux.y;
				bonus = rand() % 10;
			}
			break;
		}
		it++;
	}
	if (collision) bricks.erase(bricks.begin() + it);
	it = 0;
	collision = false;

	
	if (mouse >= 1) ballPosition.first += (deltaTimeSeconds * speed * ballMove.second);
	for (auto aux : bricks) {
		if (checkCollisionBrick(ballPosition, ballRadius, brHeight, brWidth, aux)) {
			ballMove.first = -ballMove.first;
			collision = true;
			anime.push_back(aux);
			if (yesBonus == 0) {
				bonusPosition.first = aux.x;
				bonusPosition.second = aux.y;
				bonus = rand() % 10;
			}
			break;
		}
		it++;
	}

	if (collision) bricks.erase(bricks.begin() + it);
	it = 0;
	collision = false;


	for (auto &aux : anime) {
		if(aux.scale > 0) aux.scale = aux.scale - 0.1f;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(aux.x, aux.y);
		modelMatrix *= Transform2D::Scale(aux.scale, aux.scale);
		RenderMesh2D(meshes[aux.type], shaders["VertexColor"], modelMatrix);
	}

	for (auto aux : anime) {
		if (aux.scale <= 0) {
			collision = true;
			break;
		}
		it++; 
	}

	if (collision) anime.erase(anime.begin() + it);
	collision = false;
	

	if (bonus == 1) {
		yesBonus = 1;
		modelMatrix = glm::mat3(1);
		bonusPosition.second -= deltaTimeSeconds * 400;
		modelMatrix *= Transform2D::Translate(bonusPosition.first, bonusPosition.second);
		modelMatrix *= Transform2D::Rotate(rotateangle += (deltaTimeSeconds * 10));
		RenderMesh2D(meshes["bonus"], shaders["VertexColor"], modelMatrix);
	}

	if (bonusPosition.second < 20) {
		yesBonus = 0;
		bonus = 0;
	}


	if (mouse >= 1) {
		ballPosition.second -= (deltaTimeSeconds * speed * ballMove.second);
		ballPosition.first -= (deltaTimeSeconds * speed * ballMove.second);
	}


	if (!mouse || game == 2) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(cursorPosition.x, 80);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		
	}
	else if (game == 1){
		modelMatrix = glm::mat3(1);
		speed += 0.25f;
		ballPosition.first += deltaTimeSeconds * speed * ballMove.first * forStart;
		ballPosition.second += deltaTimeSeconds * speed * ballMove.second ;
		modelMatrix *= Transform2D::Translate(ballPosition.first, ballPosition.second);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		
	}
	else if (mouse == 1 && game == 0) {
		modelMatrix = glm::mat3(1);
		ballPosition.first = cursorPosition.x;
		modelMatrix *= Transform2D::Translate(cursorPosition.x, 80);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		aux = modelMatrix;
		mouse++;
		game++;
	}
	
	
	if (checkCollisionSideWalls(ballPosition, ballRadius)) {
		ballMove.first = -ballMove.first;
	}

	if (checkCollisionTopBotWalls(ballPosition, ballRadius, botWall)) {
		ballMove.second = -ballMove.second;
	}

	if (checkCollisionPlatform(ballPosition, ballRadius, platformHeight, platformWidth, platformaX, 30)) {
		forStart = 1;
		float ip = platformWidth / 2 + ballRadius;
		float x = platformaX - ballPosition.first;
		float y = sqrt(ip*ip - x * x);
		ballMove.first = - (x / ip);
		ballMove.second = y / ip;
	
	}
	
	if (checkCollisionBonus(bonusPosition, 20, platformHeight, platformWidth, platformaX, 30)) {
		botWall = 1;	
		yesBonus = 0;
	}

	if (botWall > 0) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, 0);
		RenderMesh2D(meshes["topWall"], shaders["VertexColor"], modelMatrix);
		botWall++;
	}

	if (botWall > 200) {
		botWall = 0;
	}
	
}

void Brickbreaker::FrameEnd()
{

}

void Brickbreaker::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Brickbreaker::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Brickbreaker::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Brickbreaker::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Brickbreaker::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{	
	mouse = 1;
	// add mouse button press event
}

void Brickbreaker::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Brickbreaker::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Brickbreaker::OnWindowResize(int width, int height)
{
}
