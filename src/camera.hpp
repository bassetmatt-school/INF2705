#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
	public:
	enum Mode {
		FIRST_PERSON,
		THIRD_PERSON
	};
	public:
	Camera(glm::vec3 const& position, glm::vec2 const& orientation);

	glm::mat4 getFirstPersonViewMatrix();
	glm::mat4 getThirdPersonViewMatrix();

	Mode m_mode = THIRD_PERSON;
	private:
	const glm::vec3& m_position;
	const glm::vec2& m_orientation;
};


#endif // CAMERA_H
