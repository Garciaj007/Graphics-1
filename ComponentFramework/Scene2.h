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
	GameObject* cubeGOPtr = nullptr;
	GameObject* sphereGOPtr = nullptr;
	Mesh* cubeMeshPtr = nullptr;
	Mesh* sphereMeshPtr = nullptr;
	Shader* reflectionShaderPtr = nullptr;
	Shader* refractionShaderPtr = nullptr;
	Skybox* skybox = nullptr;

	Vec2 input = {0.f, 0.f};
	Vec2 previous = {0.f, 0.f};
	Vec2 current = {0.f, 0.f};
	Vec2 offset = {0.f, 0.f};
	bool firstGather = true;
public:
	Scene2() = default;
	virtual ~Scene2() = default;

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
};
