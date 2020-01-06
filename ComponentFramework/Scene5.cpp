#include "Scene5.h"
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

bool Scene5::OnCreate() {
	camera = new Camera();

	skybox = new Skybox();
	skybox->setProjectionMatrix(camera->getProjectionMatrix());
	skybox->setViewMatrix(camera->getViewMatrix());
	if (!skybox->OnCreate()) return false;

	if(!ObjLoader::loadOBJ("cube.obj")) return false;
	
	cubeMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	sdfShaderPtr = new Shader("customSDFVert.glsl", "customSDFFrag.glsl");
	if (!cubeMeshPtr || !sdfShaderPtr) 
	{
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	sdfGOPtr = new GameObject(cubeMeshPtr, sdfShaderPtr, nullptr);
	if (!sdfGOPtr) 
	{
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void Scene5::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (cubeMeshPtr) delete cubeMeshPtr, cubeMeshPtr = nullptr;
	if (sdfShaderPtr) delete sdfShaderPtr, sdfShaderPtr = nullptr;
	if (sdfGOPtr) delete sdfGOPtr, sdfGOPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene5::HandleEvents(const SDL_Event& sdlEvent) { }

void Scene5::Update(const float deltaTime_) {
	static auto rotation = 0.0f;
	sdfGOPtr->Update(deltaTime_);
	sdfGOPtr->setModelMatrix(MMath::scale(2.0f, 2.0f, 2.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)));
	time += deltaTime_;
	rotation += 1.0f;
}

void Scene5::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//const auto inputSpeed = 10.0f;
	auto viewMatix = camera->getViewMatrix();// * MMath::rotate(offset.x * static_cast<float>(DEGREES_TO_RADIANS) * inputSpeed, 0.f, 1.f, 0.f);

	//Drawing Skybox
	skybox->setViewMatrix(viewMatix);
	skybox->Render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	
	/// Draw your scene here
	sdfGOPtr->getShader()->useShader();
	glUniformMatrix4fv(sdfGOPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sdfGOPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatix);
	glUniform3fv(sdfGOPtr->getShader()->getUniformID("cameraLocation"), 1, (camera->getCameraPosition()));
	sdfGOPtr->Render();
	
	glUseProgram(0);
}