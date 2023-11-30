#include "Camera.hpp"
#include "glm/glm.hpp"

Camera::Camera()
{
	position = {0.f, 0.f, 0.f};	
	up = {0.f, 1.f, 0.f};	
	front = pfm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = SPEED;
	mouseSensitivity = SENSITIVITY;
	zoom = ZOOM;
	worldUp = up;
	yaw = YAW;
	pitch = PITCH;
	_updateCameraVectors();
}

pfm::mat4 Camera::getViewMatrix()
{
	return pfm::lookAt(position, position + front, up);
}

void Camera::_updateCameraVectors()
{
	pfm::vec3 direction;
	direction.x = cos(pfm::radians(yaw)) * cos(pfm::radians(pitch));
	direction.y = sin(pfm::radians(pitch));
	direction.z = sin(pfm::radians(yaw)) * cos(pfm::radians(pitch));
	this->front = pfm::normalize(direction);
	// also re-calculate the Right and Up vector
	right = pfm::normalize(pfm::cross(worldUp, front));
	up    = pfm::normalize(pfm::cross(front, right));
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position += right * velocity;
	if (direction == RIGHT)
		position -= right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	//  when pitch  out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	_updateCameraVectors();
}
