#include "Camera.h"

const float DEF_FOV = 45.0f;

Camera::Camera()
	: mPosition(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(0.0f, 0.0f, 0.0f),
	WORLD_UP(0.0f, 1.0f, 0.0f),
	mYaw(glm::pi<float>()),
	mPitch(0.0f),
	mFOV(DEF_FOV)
{
}

const glm::vec3& Camera::GetLook() const
{
	return mLook;
}

const glm::vec3& Camera::GetRight() const
{
	return mRight;
}

const glm::vec3& Camera::GetUp() const
{
	return mUp;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

/// FPSCamera

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
};

void FPSCamera::SetPosition(const glm::vec3& position) 
{
	mPosition = position;
};
void FPSCamera::Rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	UpdateCameraVectors();
};
void FPSCamera::Move(const glm::vec3& offsetPosition)
{
	mPosition += offsetPosition;
	UpdateCameraVectors();
};

void FPSCamera::UpdateCameraVectors()
{
	glm::vec3 look;

	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
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