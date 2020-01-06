#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Matrix.h"
using namespace MATH;
class Camera {
private:
	Matrix4 projection;
	Matrix4 view;
	Matrix4 rotation, translation;
	Vec3 position;
public:
	[[nodiscard]] inline Matrix4 getProjectionMatrix() const { return projection; }
	[[nodiscard]] inline Matrix4 getViewMatrix() const { return view; }
	[[nodiscard]] inline Matrix4 getCameraRotation() const { return rotation; }
	[[nodiscard]] inline Vec3 getCameraPosition() const { return position; }

	void createProjection(float fovy, float aspectRatio, float near, float far);
	void createView(Vec3 pos, Vec3 at, Vec3 up);
	Camera();
	~Camera();
};

#endif



