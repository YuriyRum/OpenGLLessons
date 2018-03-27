#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

class Camera
{
public:
	
	glm::mat4 GetViewMatrix() const;

	virtual void SetPosition(const glm::vec3& position) {};
	virtual void Rotate(float yaw, float pitch) {};
	virtual void Move(const glm::vec3& offsetPosition) {};

	const glm::vec3& GetLook() const;
	const glm::vec3& GetRight() const;
	const glm::vec3& GetUp() const;

	float GetFOV() const { return mFOV; };
	void SetFOV(float fov) { mFOV = fov; };

protected:
	Camera();

	virtual void UpdateCameraVectors() {};

	float mYaw;
	float mPitch;
	float mFOV;

	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
	glm::vec3 mLook;
	glm::vec3 mRight;

	const glm::vec3 WORLD_UP;
	
};

class FPSCamera : public Camera
{
public:
	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);

	virtual void SetPosition(const glm::vec3& position);
	virtual void Rotate(float yaw, float pitch);
	virtual void Move(const glm::vec3& offsetPosition);

private:
	void UpdateCameraVectors();
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