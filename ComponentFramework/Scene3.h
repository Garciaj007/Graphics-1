#pragma once

#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include <glew.h>

using namespace MATH;

union SDL_Event;
class Camera;
class GameObject;
class Mesh;
class Shader;
class Texture;
class Skybox;

class Scene3 final : public Scene {
private:
	Matrix4 rotationMatrix;

	Camera* camera = nullptr;
	GameObject* waterGOPtr = nullptr;
	Mesh* sphereMeshPtr = nullptr;
	Shader* waterShaderPtr = nullptr;
	Skybox* skybox = nullptr;

	GLuint perlinVolume = 0;
	Vec2 offset = {0.f, 0.f};
	float time = 0.0f;
public:
	Scene3() = default;
	virtual ~Scene3() = default;

	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event &sdlEvent) override;
	void Update(const float deltaTime_) override;
	void Render() const override;
};
