#include "Scene3.h"
#include "Debug.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "MMath.h"
#include <SDL_image.h>
#include "glm/gtc/type_ptr.hpp"
#include "Perlin.h"

bool Scene3::OnCreate() {
	camera = new Camera();

	skybox = new Skybox();
	skybox->setProjectionMatrix(camera->getProjectionMatrix());
	skybox->setViewMatrix(camera->getViewMatrix());
	if (!skybox->OnCreate()) return false;

	if(!ObjLoader::loadOBJ("sphere.obj")) return false;
	sphereMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	
	waterShaderPtr = new Shader("waterVert.glsl", "waterFrag.glsl");
	
	if (!sphereMeshPtr || !waterShaderPtr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	waterGOPtr = new GameObject(sphereMeshPtr, waterShaderPtr, nullptr);
	if (!waterGOPtr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	perlinVolume = CreateNoise3D();

	return true;
}

void Scene3::OnDestroy() {
	if (camera) delete camera, camera = nullptr;
	if (sphereMeshPtr) delete sphereMeshPtr, sphereMeshPtr = nullptr;
	if (waterShaderPtr) delete waterShaderPtr, waterShaderPtr = nullptr;
	if (waterGOPtr) delete waterGOPtr, waterGOPtr = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene3::HandleEvents(const SDL_Event& sdlEvent) { }

void Scene3::Update(const float deltaTime_) {
	static auto rotation = 0.0f;
	waterGOPtr->Update(deltaTime_);
	waterGOPtr->setModelMatrix(MMath::scale(3.5f, 3.5f, 3.5f) * MMath::rotate(rotation, Vec3(0.0, 1.0f, 0.0)) * MMath::rotate(-90, Vec3(1, 0, 0)));
	//rotation += 0.5f;
	time += deltaTime_;
	offset.x += 1;
}

void Scene3::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto inputSpeed = 10.0f;

	auto viewMatix = camera->getViewMatrix() * 
		MMath::rotate(offset.x * static_cast<float>(DEGREES_TO_RADIANS) * inputSpeed, 0.f, 1.f, 0.f);

	//Drawing Skybox
	skybox->setViewMatrix(viewMatix);
	skybox->Render();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, perlinVolume);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	
	/// Draw your scene here
	waterGOPtr->getShader()->useShader();
	glUniformMatrix4fv(waterGOPtr->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(waterGOPtr->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatix);
	glUniform1f(waterGOPtr->getShader()->getUniformID("time"), time);
	glUniform3fv(waterGOPtr->getShader()->getUniformID("light.direction"), 1, glm::value_ptr(glm::normalize(glm::vec3(1.0f))));
	glUniform3fv(waterGOPtr->getShader()->getUniformID("light.ambient"), 1, glm::value_ptr(glm::vec3(0.01f)));
	glUniform3fv(waterGOPtr->getShader()->getUniformID("light.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f)));
	glUniform3fv(waterGOPtr->getShader()->getUniformID("light.specular"), 1, glm::value_ptr(glm::vec3(0.2f)));
	glUniform1f(waterGOPtr->getShader()->getUniformID("light.fresnelPower"), 1.0f);
	glUniform1f(waterGOPtr->getShader()->getUniformID("water.rrLerpAmt"), 0.5f);
	glUniform1f(waterGOPtr->getShader()->getUniformID("water.colorLerpAmt"), 0.2f);
	glUniform4fv(waterGOPtr->getShader()->getUniformID("water.color"), 1, glm::value_ptr(glm::vec4(9.f/255.f, 62.f/255.f, 73.f/255.f, 0.5f)));
	waterGOPtr->Render();
	
	glUseProgram(0);
}