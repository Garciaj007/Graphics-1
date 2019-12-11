#pragma once

#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
using namespace MATH;

union SDL_Event;
class Camera;
class GameObject;
class Mesh;
class Shader;
class Texture;
class Skybox;

class Scene2 final : public Scene {
public:
	Scene2() = default;
	virtual ~Scene2() = default;

	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event &sdlEvent) override;
	void Update(const float deltaTime_) override;
	void Render() const override;
	
private:
	Matrix4 rotationMatrix;
	
	Camera* camera = nullptr;
	GameObject* cubeGOPtr = nullptr;
	GameObject* sphereGOPtr = nullptr;
	Mesh* cubeMeshPtr = nullptr;
	Mesh* sphereMeshPtr = nullptr;
	Shader* reflectionShaderPtr = nullptr;
	Shader* refractionShaderPtr = nullptr;
	Skybox* skybox = nullptr;

	Vec2 offset = {0.f, 0.f};
};
