#include <SDL.h>
#include <vector>
#include "Scene6.h"
#include "Debug.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "MMath.h"
#include "glm/gtc/type_ptr.hpp"
#include "GameSceneManager.h"

#define VERT "Shaders/Vert.glsl"
#define FRAG "Shaders/Frag.glsl"

bool Scene6::OnCreate() {
	camera = new Camera();

	if (!ObjLoader::loadOBJ("cube.obj")) return false;

	cubeMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/Frag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/linearMouseFrag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/2DRaymarchingFrag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/RaymarchingLakeFrag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/SeaofDiamondsFrag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/Frag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/Frag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/Frag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/Frag.glsl"));
	shaderList.push_back(new Shader("Shaders/Vert.glsl", "Shaders/Frag.glsl"));

	if (!cubeMeshPtr || shaderList.empty())
	{
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	shaderIndex = 0;
	gameObjectPtr = new GameObject(cubeMeshPtr, shaderList[shaderIndex], nullptr);
	if (!gameObjectPtr)
	{
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void Scene6::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (cubeMeshPtr) delete cubeMeshPtr, cubeMeshPtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (gameObjectPtr) delete gameObjectPtr, gameObjectPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
	for (auto shader : shaderList)
	{
		delete shader;
		shader = nullptr;
	}
	shaderList.clear();
}

void Scene6::HandleEvents(const SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&mouseX, &mouseY);
	}

	if (sdlEvent.type == SDL_KEYUP)
	{
		switch (sdlEvent.key.keysym.scancode)
		{
			case SDL_SCANCODE_0:  shaderIndex = 0; break;
			case SDL_SCANCODE_1:  shaderIndex = 1; break;
			case SDL_SCANCODE_2:  shaderIndex = 2; break;
			case SDL_SCANCODE_3:  shaderIndex = 3; break;
			case SDL_SCANCODE_4:  shaderIndex = 4; break;
			case SDL_SCANCODE_5:  shaderIndex = 5; break;
			case SDL_SCANCODE_6:  shaderIndex = 6; break;
			case SDL_SCANCODE_7:  shaderIndex = 7; break;
			case SDL_SCANCODE_8:  shaderIndex = 8; break;
			case SDL_SCANCODE_9:  shaderIndex = 9; break;
		}
	}
}

void Scene6::Update(const float deltaTime_) {
	static auto rotation = 0.0f;
	gameObjectPtr->Update(deltaTime_);
	gameObjectPtr->setModelMatrix(MMath::scale(2.0f, 2.0f, 2.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)));
	time += deltaTime_;
	rotation += 1.0f;
}

void Scene6::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Draw your scene here
	gameObjectPtr->setShader(shaderList[shaderIndex]);
	gameObjectPtr->getShader()->useShader();
	glUniformMatrix4fv(gameObjectPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(gameObjectPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(gameObjectPtr->getShader()->getUniformID("cameraLocation"), 1, (camera->getCameraPosition()));

	//std::string info = "Resolution [";
	//info += std::to_string(GameSceneManager::width);
	//info += ", ";
	//info += std::to_string(GameSceneManager::height);
	//info += "]\tMouse Position [";
	//info += std::to_string(mouseX);
	//info += ", ";
	//info += std::to_string(mouseY);
	//info += "]";
	//std::cout << info << std::endl;

	glUniform2f(gameObjectPtr->getShader()->getUniformID("resolution"), static_cast<float>(GameSceneManager::width), static_cast<float>(GameSceneManager::height));
	glUniform2f(gameObjectPtr->getShader()->getUniformID("mouse"), static_cast<float>(mouseX), static_cast<float>(GameSceneManager::height - mouseY));
	glUniform1f(gameObjectPtr->getShader()->getUniformID("time"), time);

	gameObjectPtr->Render();

	glUseProgram(0);
}