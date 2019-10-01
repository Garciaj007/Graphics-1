#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Matrix.h"
using namespace MATH;
class Camera {
private:
	class Skybox* skyboxPtr;
	Matrix4 projection;
	Matrix4 view;
	Matrix4 rotation, translation;
public:
	inline Matrix4 getProjectionMatrix() const { return projection; }
	inline Matrix4 getViewMatrix() const { return view; }
	inline Matrix4 getCameraRotation() const { return rotation; }

	void createProjection(float fovy, float aspectRatio, float near, float far);
	void createView(Vec3 pos, Vec3 at, Vec3 up);
	Camera();
	~Camera();
};

#endif



