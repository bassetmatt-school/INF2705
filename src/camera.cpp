#include "camera.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 const& position, glm::vec2 const& orientation) :
	m_position(position),
	m_orientation(orientation) {}

glm::mat4 Camera::getFirstPersonViewMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	float theta = glm::radians(m_orientation.x);
	float phi = glm::radians(m_orientation.y);
	view *= glm::rotate(
		glm::mat4(1.0f),
		glm::radians(m_orientation.x),
		glm::vec3(0., 1., 0.)
	);
	view *= glm::rotate(
		glm::mat4(1.0f),
		glm::radians(m_orientation.y),
		glm::vec3(glm::cos(theta), 0., glm::sin(theta))
	);
	view *= glm::translate(
		glm::mat4(1.0f),
		-m_position
	);
	return view;
}

glm::mat4 Camera::getThirdPersonViewMatrix() {
	//TODO Better
	return glm::mat4(1.0f);
}
