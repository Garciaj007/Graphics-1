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
	Skybox();
	~Skybox();

	bool OnCreate();
	bool LoadTextures(std::vector<std::string>);
	void OnDestroy();
	void Render()const;

	inline GLuint getTextureID() const { return textureID; }
	inline Shader* getShader() const { return skyboxShader; }
	inline void setProjectionMatrix(Matrix4 proj) { projectionMatrix = proj; }
	inline void setViewMatrix(Matrix4 view) { viewMatrix = view; }
private:
	GLuint textureID;
	Mesh* cubeMap;
	Shader* skyboxShader;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
};
