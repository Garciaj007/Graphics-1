#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Vector.h"
#include <vector>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Camera;
class GameObject;
class Mesh;
class Shader;
class Texture;

class Scene0 : public Scene {
private:
	Camera *camera;
	GameObject *gameObject;
	std::vector<Vec3> lightSources;
	Mesh *meshPtr;
	Shader *shaderPtr;
	Texture *texturePtr;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H