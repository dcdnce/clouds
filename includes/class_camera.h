#pragma once

#include "pfm/pfm.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  5.f;
const float SENSITIVITY =  0.15f;
const float ZOOM        =  45.0f;

class Camera {
	private:
		void _UpdateCameraVectors() noexcept;

	public:
		// camera Attributes
		pfm::vec3 position;
		pfm::vec3 front;
		pfm::vec3 up;
		pfm::vec3 right;
		pfm::vec3 world_up;
		// euler Angles
		float yaw;
		float pitch;
		// camera options
		float movement_speed;
		float mouse_sensitivity;
		float zoom;

		Camera();

		void ProcessKeyboard(Camera_Movement direction, float delta_time) noexcept;
		void ProcessMouseMovement(float xoffset, float yoffset) noexcept;

		inline pfm::mat4 GetViewMatrix() const noexcept
		{
			return pfm::lookAt(position, position + front, up);
		}

};

std::ostream& operator<<(std::ostream& o, Camera const& camera) noexcept;
