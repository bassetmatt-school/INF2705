#include <iostream>
#include <cstdio>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "camera.hpp"
#include "scenes.hpp"

#include "utils.hpp"

void printGLInfo();

int main() {
	Window w;
	if (!w.init())
		return -1;

	GLenum rev = glewInit();
	if (rev != GLEW_OK) {
		std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
		return -2;
	}
	printGLInfo();

	// RESOURCES

	Resources res;

	// CONTEXT PARAMS

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// CAMERA AND PLAYER PROPERTIES

	glm::vec3 playerPosition = glm::vec3(0);
	glm::vec2 playerOrientation = glm::vec2(0);
	Camera c(playerPosition, playerOrientation);

	bool isFirstPersonCam = false;
	bool isMouseMotionEnabled = true;
	int scrollLevel = 8;

	// SCENES

	WorldScene worldScene(res, isFirstPersonCam, playerPosition, playerOrientation);
	StencilTestScene stencilScene(res, isFirstPersonCam, playerPosition, playerOrientation);
	LightingTestScene lightingScene(res);

	enum class AvailableScene {
		WORLD, STENCIL_TEST, LIGHTING_TEST
	} sceneChoice = AvailableScene::LIGHTING_TEST;

	bool isRunning = true;
	while (isRunning) {
		GL_CHECK_ERROR;
		if (w.shouldResize())
			glViewport(0, 0, w.getWidth(), w.getHeight());

		// Clears buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float time = w.getTick() / 1000.0;

		// PROCESS INPUT

		if (w.getKeyPress(Window::Key::T)) {
			switch (sceneChoice) {
				case AvailableScene::WORLD: sceneChoice = AvailableScene::STENCIL_TEST; break;
				case AvailableScene::STENCIL_TEST: sceneChoice = AvailableScene::LIGHTING_TEST; break;
				case AvailableScene::LIGHTING_TEST: sceneChoice = AvailableScene::STENCIL_TEST; break;
			}
		}

		if (w.getKeyPress(Window::Key::SPACE)) isMouseMotionEnabled = !isMouseMotionEnabled;

		const int SCROLL_MIN = 1, SCROLL_MAX = 12;
		scrollLevel -= w.getMouseScrollDirection();
		scrollLevel = glm::clamp(scrollLevel, SCROLL_MIN, SCROLL_MAX);
		isFirstPersonCam = scrollLevel == SCROLL_MIN;

		if (isMouseMotionEnabled) w.hideMouse();
		else w.showMouse();

		if (isMouseMotionEnabled) {
			int x = 0, y = 0;
			w.getMouseMotion(x, y);
			playerOrientation.y -= x * 0.01f;
			playerOrientation.x -= y * 0.01f;
		}
		glm::vec3 positionOffset = glm::vec3(0.0);
		float speed = 0.1f;
		// Sprint feature, can be useful to navigate the terrain
		if (w.getKeyHold(Window::Key::SHIFT) || w.getKeyHold(Window::Key::BUTTON5)) speed *= 1.8;
		if (w.getKeyHold(Window::Key::W)) positionOffset.z -= speed;
		if (w.getKeyHold(Window::Key::S)) positionOffset.z += speed;
		if (w.getKeyHold(Window::Key::A)) positionOffset.x -= speed;
		if (w.getKeyHold(Window::Key::D)) positionOffset.x += speed;
		// Upward and downward movement to check from above to have a global
		// view or below to check face culling
		if (w.getKeyHold(Window::Key::Q)) positionOffset.y += speed;
		if (w.getKeyHold(Window::Key::E)) positionOffset.y -= speed;

		positionOffset = glm::rotate(glm::mat4(1.0f), playerOrientation.y, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(positionOffset, 1);

		if (sceneChoice != AvailableScene::LIGHTING_TEST)
			playerPosition += positionOffset;

		glm::mat4 projPersp = glm::perspective(glm::radians(70.0f), (float) w.getWidth() / (float) w.getHeight(), 0.1f, 200.0f);
		glm::mat4 view = isFirstPersonCam ? c.getFirstPersonViewMatrix() : c.getThirdPersonViewMatrix(scrollLevel);


		switch (sceneChoice) {
			case AvailableScene::WORLD:         worldScene.render(view, projPersp);    break;
			case AvailableScene::STENCIL_TEST:  stencilScene.render(view, projPersp); break;
			case AvailableScene::LIGHTING_TEST: lightingScene.render(view, projPersp); break;
		}
		w.swap();
		w.pollEvent();
		isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
	}

	return 0;
}

void printGLInfo() {
	std::cout << "OpenGL info:" << std::endl;
	std::cout << "    Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
