#include "Scene2.h"
#include <glew.h>
#include "Debug.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "MMath.h"
#include <SDL_image.h>

bool Scene2::OnCreate() {
	camera = new Camera();

	skybox = new Skybox();
	skybox->setProjectionMatrix(camera->getProjectionMatrix());
	skybox->setViewMatrix(camera->getViewMatrix());
	if (!skybox->OnCreate()) return false;

	if (!ObjLoader::loadOBJ("cube.obj")) return false;
	cubeMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	if(!ObjLoader::loadOBJ("sphere.obj")) return false;
	sphereMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	
	reflectionShaderPtr = new Shader("reflectionVert.glsl", "reflectionFrag.glsl");
	refractionShaderPtr = new Shader("refractionVert.glsl", "refractionFrag.glsl");
	
	if (!cubeMeshPtr || !sphereMeshPtr || !reflectionShaderPtr || !refractionShaderPtr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	cubeGOPtr = new GameObject(sphereMeshPtr, reflectionShaderPtr, nullptr);
	sphereGOPtr = new GameObject(sphereMeshPtr, refractionShaderPtr, nullptr);
	if (!cubeGOPtr || !sphereGOPtr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void Scene2::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (cubeMeshPtr) delete cubeMeshPtr, cubeMeshPtr = nullptr;
	if (sphereMeshPtr) delete sphereMeshPtr, sphereMeshPtr = nullptr;
	if (reflectionShaderPtr) delete reflectionShaderPtr, reflectionShaderPtr = nullptr;
	if (refractionShaderPtr) delete refractionShaderPtr, refractionShaderPtr = nullptr;
	if (cubeGOPtr) delete cubeGOPtr, cubeGOPtr = nullptr;
	if (sphereGOPtr) delete sphereGOPtr, sphereGOPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent) { }

void Scene2::Update(const float deltaTime_) {
	static auto rotation = 0.0f;
	sphereGOPtr->Update(deltaTime_);
	sphereGOPtr->setModelMatrix(MMath::scale(1.5f, 1.5f, 1.5f) * MMath::translate(2.0f, 0.5f, 0.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(-90, Vec3(1, 0, 0)));
	cubeGOPtr->Update(deltaTime_);
	cubeGOPtr->setModelMatrix(MMath::scale(1.5f, 1.5f, 1.5f) * MMath::translate(-2.0f, -0.5f, 0.0f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(-90, Vec3(1, 0, 0)));
	rotation += 0.5f;
	offset.x += 1;
}

void Scene2::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto inputSpeed = 10.0f;

	auto viewMatix = camera->getViewMatrix() * 
		MMath::rotate(offset.x * static_cast<float>(DEGREES_TO_RADIANS) * inputSpeed, 0.f, 1.f, 0.f);

	//Drawing Skybox
	skybox->setViewMatrix(viewMatix);
	skybox->Render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	
	/// Draw your scene here
	cubeGOPtr->getShader()->useShader();
	/// These pass the matrices and the light position to the GPU
	glUniformMatrix4fv(cubeGOPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(cubeGOPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatix);
	glUniform3f(cubeGOPtr->getShader()->getUniformID("cameraPos"), 0, 0, 0);
	cubeGOPtr->Render();

	sphereGOPtr->getShader()->useShader();
	glUniformMatrix4fv(sphereGOPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sphereGOPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatix);
	glUniform3f(sphereGOPtr->getShader()->getUniformID("cameraPos"), 0, 0, 0);
	sphereGOPtr->Render();
	
	glUseProgram(0);
}


