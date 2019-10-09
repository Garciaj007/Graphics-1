#include "Skybox.h"
#include <SDL_image.h>
#include "Debug.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "MMath.h"

Skybox::Skybox() { }

Skybox::~Skybox()
{
	OnDestroy();
}

bool Skybox::OnCreate()
{
	if (!ObjLoader::loadOBJ("cube.obj")) return false;

	cubeMap = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	if (!cubeMap) return false;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	if (!LoadTextures({ "right.png", "left.png", "top.png", "bottom.png", "front.png", "back.png"})) 
	{
		Debug::FatalError("Couldn't load cubemap textures", __FILE__, __LINE__);
		return false;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	skyboxShader = new Shader("skyboxVert.glsl", "skyboxFrag.glsl");

	if (!skyboxShader) return false;

	return true;
}

bool Skybox::LoadTextures(std::vector<std::string> paths)
{
	for (auto i = 0; i < paths.size(); i++) {
		auto data = IMG_Load(paths[i].c_str());
		if (data) {
			int mode = (data->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, data->w, data->h, 0, mode, GL_UNSIGNED_BYTE, data->pixels);
			SDL_FreeSurface(data);
		}
		else {
			Debug::Error("Cubemap texture failed to load at path: " + paths[i], __FILE__, __LINE__);
			SDL_FreeSurface(data);
			return false;
		}
	}
	return true;
}

void Skybox::OnDestroy()
{
	if (cubeMap) delete cubeMap, cubeMap = nullptr;
	if (skyboxShader) delete skyboxShader, skyboxShader = nullptr;
}

void Skybox::Render() const
{
	glDepthMask(GL_FALSE);

	Matrix4 modelMatrix = MMath::scale(10.f, 10.f, 10.f);
	glUniformMatrix4fv(skyboxShader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(skyboxShader->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	cubeMap->Render();
	
	glDepthMask(GL_TRUE);
}