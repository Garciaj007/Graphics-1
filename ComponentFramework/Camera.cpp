#include "Camera.h"
#include "MMath.h"
#include "Skybox.h"

Camera::Camera() {
	createProjection(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	createView(Vec3(0.0, 0.0, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	skyboxPtr = new Skybox();
}

Camera::~Camera() { if (skyboxPtr) delete skyboxPtr, skyboxPtr = nullptr; }

void Camera::createProjection(float fovy, float aspect, float near, float far) {
	projection = MMath::perspective(fovy, aspect, near, far);
}

void Camera::createView(Vec3 pos, Vec3 at, Vec3 up) {
	rotation = MMath::rotate(-15.0f, Vec3(0, 1, 0));
	translation = MMath::translate(0.f, 0.f, -10.f);
	view = rotation * translation;
	skyboxPtr->setProjectionMatrix(view);
	skyboxPtr->Render();
}