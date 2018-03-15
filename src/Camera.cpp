#include "Camera.h"

Camera::Camera()
	: mPosition(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mYaw(0.0f),
	mPitch(0.0f)
{

}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

/// Orbit Camera implementation

OrbitCamera::OrbitCamera()
	:mRadius(10.0f)
{

}

void OrbitCamera::Rotate(float yaw, float pitch)
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	UpdateCameraVectors();
}

void OrbitCamera::SetLookAt(glm::vec3& vec)
{
	mTargetPos = vec;
}

void OrbitCamera::SetRadius(float radius)
{
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::UpdateCameraVectors()
{
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}