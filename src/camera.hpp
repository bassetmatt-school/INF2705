#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
	public:
	Camera(glm::vec3 const& position, glm::vec2 const& orientation);

	glm::mat4 getFirstPersonViewMatrix();
	glm::mat4 getThirdPersonViewMatrix();

	private:
	const glm::vec3& m_position;
	const glm::vec2& m_orientation;
};


#endif // CAMERA_H
