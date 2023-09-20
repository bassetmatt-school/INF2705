#include "camera.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 const& position, glm::vec2 const& orientation) :
	m_position(position),
	m_orientation(orientation) {}

glm::mat4 Camera::getFirstPersonViewMatrix() {
	//TODO Better
	return glm::mat4(1.0f);
}

glm::mat4 Camera::getThirdPersonViewMatrix() {
	//TODO Better
	return glm::mat4(1.0f);
}
