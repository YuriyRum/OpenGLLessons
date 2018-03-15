#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	
	glm::mat4 GetViewMatrix() const;

protected:
	Camera();

	float mYaw;
	float mPitch;

	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
};

class OrbitCamera : public Camera 
{
public:
	OrbitCamera();

	virtual void Rotate(float yaw, float pitch);

	void SetLookAt(glm::vec3& cev);
	void SetRadius(float radius);

private:
	float mRadius;

	void UpdateCameraVectors();
};