#pragma once
#include <vector>
#include <string>
#include <glew.h>
#include "Matrix.h"

class Shader;
class Mesh;

using namespace MATH;

class Skybox 
{
public:
	Skybox() = default;
	~Skybox();

	bool OnCreate();
	bool LoadTextures(std::vector<std::string>);
	void OnDestroy();
	void Render()const;

	GLuint getTextureID() const { return cubemapTextureId; }
	Shader* getShader() const { return skyboxShader; }
	void setProjectionMatrix(const Matrix4 proj) { projectionMatrix = proj; }
	void setViewMatrix(const Matrix4 view) { viewMatrix = view; }
private:
	GLuint cubemapTextureId;
	Mesh* cubeMap;
	Shader* skyboxShader;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
};
