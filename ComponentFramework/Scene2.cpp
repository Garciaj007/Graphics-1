#include "Scene2.h"
#include <glew.h>
#include <iostream>
#include <string>
#include "Window.h"
#include "Debug.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include <SDL_image.h>

Scene2::Scene2() {}

Scene2::~Scene2() {}

bool Scene2::OnCreate() {
	camera = new Camera();

	skybox = new Skybox();
	skybox->setProjectionMatrix(camera->getProjectionMatrix());
	skybox->setViewMatrix(camera->getViewMatrix());
	if (!skybox->OnCreate()) return false;

	if (ObjLoader::loadOBJ("cube.obj") == false)
		return false;

	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	shaderPtr = new Shader("reflectionVert.glsl", "reflectionFrag.glsl");

	if (meshPtr == nullptr|| shaderPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	modelGameObjectPtr = new GameObject(meshPtr, shaderPtr, nullptr);

	if (modelGameObjectPtr == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void Scene2::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (modelGameObjectPtr) delete modelGameObjectPtr, modelGameObjectPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene2::HandleEvents(const SDL_Event &sdlEvent) {}

void Scene2::Update(const float deltaTime_) {
	static float rotation = 0.0f;
	static float orbitSpeed = 0.0f;
	modelGameObjectPtr->Update(deltaTime_);
	modelGameObjectPtr->setModelMatrix(MMath::scale(2.0f, 2.0f, 2.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(-90, Vec3(1, 0, 0)));
	rotation += 1.5f;
	orbitSpeed += 0.05f;
}

void Scene2::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// Draw your scene here
	GLuint program = modelGameObjectPtr->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(modelGameObjectPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(modelGameObjectPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3f(modelGameObjectPtr->getShader()->getUniformID("cameraPos"), 0, 0, 0.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());

	modelGameObjectPtr->Render();

	//Drawing Skybox
	skybox->Render();

	glUseProgram(0);
}


