#include "class_camera.h"

Camera::Camera()
{
	position = {0.f, 0.f, 10.f};
	up = {0.f, 1.f, 0.f};
	front = pfm::vec3(0.0f, 0.0f, -1.0f);
	movement_speed = SPEED;
	mouse_sensitivity = SENSITIVITY;
	zoom = ZOOM;
	world_up = up;
	yaw = YAW;
	pitch = PITCH;
	_UpdateCameraVectors();
}

std::ostream& operator<<(std::ostream& o, Camera const& camera) noexcept
{
	o << "Camera :" << std::endl;
	o << "- Position: " << camera.position << std::endl;
	o << "- Up: " << camera.up << std::endl;
	o << "- Front: " << camera.front << std::endl;
	return (o);
}

pfm::mat4 Camera::GetViewMatrix() const noexcept
{
	return pfm::lookAt(position, position + front, up);
}

void Camera::_UpdateCameraVectors() noexcept
{
	pfm::vec3 direction;
	direction.x = cos(pfm::radians(yaw)) * cos(pfm::radians(pitch));
	direction.y = sin(pfm::radians(pitch));
	direction.z = sin(pfm::radians(yaw)) * cos(pfm::radians(pitch));
	this->front = pfm::normalize(direction);
	// also re-calculate the Right and Up vector
	right = pfm::normalize(pfm::cross(world_up, front));
	up    = pfm::normalize(pfm::cross(front, right));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delta_time) noexcept
{
	float velocity = movement_speed * delta_time;
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
void Camera::ProcessMouseMovement(float xoffset, float yoffset) noexcept
{
	xoffset *= mouse_sensitivity;
	yoffset *= mouse_sensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	//  when pitch  out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	_UpdateCameraVectors();
}
