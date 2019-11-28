#include <glew.h>
#include <iostream>
#include <string>
#include "Window.h"
#include "Debug.h"
#include "Scene1.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include <SDL_image.h>

Scene1::Scene1():camera(nullptr), earthGameObjectPtr(nullptr), meshPtr(nullptr) ,shaderPtr(nullptr), earthTexturePtr(nullptr), skybox(nullptr), cubemapID(0), moonTexturePtr(nullptr), moonGameObjectPtr(nullptr)  {}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	camera = new Camera();

	skybox = new Skybox();
	skybox->setProjectionMatrix(camera->getProjectionMatrix());
	skybox->setViewMatrix(camera->getViewMatrix());
	if (!skybox->OnCreate()) return false;

	if (ObjLoader::loadOBJ("sphere.obj") == false)
		return false;

	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	shaderPtr = new Shader("textureVert.glsl", "textureFrag.glsl");

	earthTexturePtr = new Texture();
	moonTexturePtr = new Texture();

	if (meshPtr == nullptr|| shaderPtr == nullptr || earthTexturePtr == nullptr || moonTexturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	if (earthTexturePtr->LoadImage("earth.jpg") == false || moonTexturePtr->LoadImage("moon.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	earthGameObjectPtr = new GameObject(meshPtr, shaderPtr, earthTexturePtr);
	moonGameObjectPtr = new GameObject(meshPtr, shaderPtr, moonTexturePtr);

	if (earthGameObjectPtr == nullptr || moonGameObjectPtr == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	light = Vec3(0, 16.0f, 0);

	return true;
}

void Scene1::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (earthTexturePtr) delete earthTexturePtr, earthTexturePtr = nullptr;
	if (moonTexturePtr) delete moonTexturePtr, moonTexturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (earthGameObjectPtr) delete earthGameObjectPtr, earthGameObjectPtr = nullptr;
	if (moonGameObjectPtr) delete moonGameObjectPtr, moonGameObjectPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {}

void Scene1::Update(const float deltaTime_) {
	static float rotation = 0.0f;
	static float orbitSpeed = 0.0f;
	angle += 10.0f;
	earthGameObjectPtr->Update(deltaTime_);
	moonGameObjectPtr->Update(deltaTime_);
	rotation += 0.5f;
	orbitSpeed += 0.05f;
	earthGameObjectPtr->setModelMatrix(MMath::scale(2.0f, 2.0f, 2.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(-90, Vec3(1, 0, 0)));
	moonGameObjectPtr->setModelMatrix(MMath::translate(4 * sinf(static_cast<float>(DEGREES_TO_RADIANS) * orbitSpeed), 0.0f, 4 * cosf(static_cast<float>(DEGREES_TO_RADIANS) * orbitSpeed)) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(90, Vec3(1, 0, 0)));
}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto viewMatrix = camera->getViewMatrix() * MMath::rotate(static_cast<float>(DEGREES_TO_RADIANS) * angle, Vec3(0.f, 1.f, 0.f));
	
	//Drawing Skybox
	skybox->setViewMatrix(viewMatrix);
	skybox->Render();

	/// Draw your scene here
	earthGameObjectPtr->getShader()->useShader();

	/// These pass the matrices and the light position to the GPU
	glUniformMatrix4fv(earthGameObjectPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(earthGameObjectPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(earthGameObjectPtr->getShader()->getUniformID("lightPos"), 1, light);
	earthGameObjectPtr->Render();
	moonGameObjectPtr->Render();

	glUseProgram(0);
}


