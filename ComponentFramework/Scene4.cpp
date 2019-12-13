#include "Scene4.h"
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

bool Scene4::OnCreate() {
	camera = new Camera();

	skybox = new Skybox();
	skybox->setProjectionMatrix(camera->getProjectionMatrix());
	skybox->setViewMatrix(camera->getViewMatrix());
	if (!skybox->OnCreate()) return false;

	if(!ObjLoader::loadOBJ("cube.obj")) return false;
	cubeMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	
	sdfShaderPtr = new Shader("sdfVert.glsl", "sdfFrag.glsl");
	
	if (!cubeMeshPtr || !sdfShaderPtr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	sdfGoPtr = new GameObject(cubeMeshPtr, sdfShaderPtr, nullptr);
	if (!sdfGoPtr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void Scene4::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (cubeMeshPtr) delete cubeMeshPtr, cubeMeshPtr = nullptr;
	if (sdfShaderPtr) delete sdfShaderPtr, sdfShaderPtr = nullptr;
	if (sdfGoPtr) delete sdfGoPtr, sdfGoPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene4::HandleEvents(const SDL_Event& sdlEvent) { }

void Scene4::Update(const float deltaTime_) {
	static auto rotation = 0.0f;
	sdfGoPtr->Update(deltaTime_);
	sdfGoPtr->setModelMatrix(MMath::scale(2.0f, 2.0f, 2.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(-90, Vec3(1, 0, 0)));
	time += deltaTime_;
	offset.x += 1.0f;
}

void Scene4::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto inputSpeed = 10.0f;
	auto viewMatix = camera->getViewMatrix() * MMath::rotate(offset.x * static_cast<float>(DEGREES_TO_RADIANS) * inputSpeed, 0.f, 1.f, 0.f);

	//Drawing Skybox
	skybox->setViewMatrix(viewMatix);
	skybox->Render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	
	/// Draw your scene here
	sdfGoPtr->getShader()->useShader();
	glUniformMatrix4fv(sdfGoPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sdfGoPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatix);
	glUniform2f(sdfGoPtr->getShader()->getUniformID("resolution"), GameSceneManager::width, GameSceneManager::height);
	glUniform1f(sdfGoPtr->getShader()->getUniformID("time"), time);
	sdfGoPtr->Render();
	
	glUseProgram(0);
}