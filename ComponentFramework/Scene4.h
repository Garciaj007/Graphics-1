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

class Scene4 final : public Scene {
private:
	Camera* camera = nullptr;
	GameObject* sdfGoPtr = nullptr;
	Mesh* cubeMeshPtr = nullptr;
	Shader* sdfShaderPtr = nullptr;
	Skybox* skybox = nullptr;

	Matrix4 rotationMatrix;
	Vec2 offset = {0.0f, 0.0f};
	float time = 0.0f;
public:
	Scene4() = default;
	virtual ~Scene4() = default;

	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event &sdlEvent) override;
	void Update(const float deltaTime_) override;
	void Render() const override;
};
