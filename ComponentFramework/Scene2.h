#pragma once

#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include <vector>
using namespace MATH;

union SDL_Event;
class Camera;
class GameObject;
class Mesh;
class Shader;
class Texture;
class Skybox;

class Scene2 : public Scene {
private:
	unsigned int cubemapID;

	Matrix4 rotationMatrix;

	Camera* camera = nullptr;
	GameObject* modelGameObjectPtr = nullptr;
	Mesh* meshPtr = nullptr;
	Shader* shaderPtr = nullptr;
	Skybox* skybox = nullptr;
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
};
