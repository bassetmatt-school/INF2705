#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "model.hpp"
#include "shader_program.hpp"
#include "shapes.hpp"
#include "utils.hpp"
#include "vertices_data.hpp"
#include "window.hpp"

constexpr int N_ROWS = 7;
constexpr int N_GROUPS = N_ROWS * N_ROWS;

void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__LINE__)
void checkGLError(int line);

std::string readFile(const char* path);

void groupInstanciation(glm::mat4* treeTransform, glm::mat4* rockTransform, glm::mat4* shroomTransform);


int main() {
	Window w;
	if (!w.init())
		return -1;

	GLenum rev = glewInit();
	if (rev != GLEW_OK) {
		std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
		return -2;
	}
	// printGLInfo();

	// Shader Program
	ShaderProgram modelShader;
	{
		std::string fragmentShaderCode = readFile("./shaders/model.fs.glsl");
		std::string vertexShaderCode   = readFile("./shaders/model.vs.glsl");
		Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode.c_str());
		Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());
		modelShader.attachShader(vertexShader);
		modelShader.attachShader(fragmentShader);
		modelShader.link();
	}
	modelShader.use();


	/****************
	* Create shapes *
	*****************/

	// TODO Remove for model
	// Cube instanciation

	BasicShapeElements ground;
	ground.setData(
		groundVertices,
		sizeof(groundVertices),
		groundIndexes,
		sizeof(groundIndexes)
	);

	BasicShapeElements river;
	river.setData(
		riverVertices,
		sizeof(riverVertices),
		riverIndexes,
		sizeof(riverIndexes)
	);

	BasicShapeElements hud;
	hud.setData(
		hudVertices,
		sizeof(hudVertices),
		hudIndexes,
		sizeof(hudIndexes)
	);

	Model suzanne("../models/suzanne.obj");
	Model tree("../models/tree.obj");
	Model mushroom("../models/mushroom.obj");
	Model rock("../models/rock.obj");

	// Groups
	glm::mat4 treeTransform[N_GROUPS];
	glm::mat4 rockTransform[N_GROUPS];
	glm::mat4 shroomTransform[N_GROUPS];

	groupInstanciation(treeTransform, rockTransform, shroomTransform);

	// Shader attributes
	GLint locMVP, locColor;
	{
		GLint locVertex = modelShader.getAttribLoc("scenePosition");
		ground.enableAttribute(locVertex, 3, 12, 0);
		river.enableAttribute(locVertex, 3, 12, 0);
		hud.enableAttribute(locVertex, 3, 12, 0);
		locColor = modelShader.getUniformLoc("color");
		locMVP = modelShader.getUniformLoc("pvmMatrix");
	}

	// Camera
	glm::vec3 playerPos(0.);
	glm::vec2 playerOrientation(0.);
	Camera camera(playerPos, playerOrientation);

	// Background color, sort of dark gray
	glm::vec4 clearColor(0.11f, 0.12f, 0.13f, 1.0f);

	// Enables depth test & Face culling
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	bool isRunning = true;
	int mouseX, mouseY;
	while (isRunning) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		// Background Color
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		if (w.shouldResize())
			glViewport(0, 0, w.getWidth(), w.getHeight());

		// Clears buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Player inputs
		w.getMouseMotion(mouseX, mouseY);
		playerOrientation.x += mouseX * 0.1;
		playerOrientation.y += mouseY * 0.1;
		// Prevents camera from going upside down
		// Limited to 89.99° to avoid bug with 3rd person camera
		playerOrientation.y = glm::clamp(playerOrientation.y, -89.99f, 89.99f);
		float theta = glm::radians(playerOrientation.x);

		glm::vec3 up(0., 1., 0.);
		glm::vec3 forward(glm::sin(theta), 0., -glm::cos(theta));
		glm::vec3 right = glm::cross(forward, up);
		if (w.getKeyHold(Window::Key::W)) playerPos += 0.07f * forward;
		if (w.getKeyHold(Window::Key::S)) playerPos -= 0.07f * forward;
		if (w.getKeyHold(Window::Key::A)) playerPos -= 0.07f * right;
		if (w.getKeyHold(Window::Key::D)) playerPos += 0.07f * right;
		//TODO: Remove
		if (w.getKeyHold(Window::Key::Q)) playerPos += 0.07f * up;
		if (w.getKeyHold(Window::Key::E)) playerPos -= 0.07f * up;
		// Switch Camera mode when wheel event
		switch (w.getMouseScrollDirection()) {
			case 1:
				camera.m_mode = Camera::Mode::FIRST_PERSON;
				break;
			case -1:
				camera.m_mode = Camera::Mode::THIRD_PERSON;
				break;
			default:	break;
		}

		// Projection matrix
		// FOV: 70°, near plane at 0.1, far plane at 10
		float aspect = w.getWidth() / (float) w.getHeight();
		glm::mat4 proj = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 100.0f);
		glm::mat4 view = glm::mat4(1.0f);

		// View matrix
		switch (camera.m_mode) {
			case Camera::Mode::FIRST_PERSON:
				view = camera.getFirstPersonViewMatrix();
				break;
			case Camera::Mode::THIRD_PERSON:
				view = camera.getThirdPersonViewMatrix();
				break;
		}
		glm::mat4 display = proj * view;


		// Drawing ground and river
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvp = display * model;
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
		glUniform3f(locColor, 0.14f, 0.60f, 0.11f);
		ground.draw(GL_TRIANGLES, 6);
		glUniform3f(locColor, 0.22f, 0.34f, 0.83f);
		river.draw(GL_TRIANGLES, 6);

		// Drawing groups
		for (int i = 0; i < N_GROUPS; ++i) {
			// Trees
			mvp = display * treeTransform[i];
			glUniform3f(locColor, 0.47f, 0.32f, 0.03f);
			glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
			tree.draw();
			// Rocks
			mvp = display * rockTransform[i];
			glUniform3f(locColor, 0.5f, 0.5f, 0.5f);
			glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
			rock.draw();
			// Mushrooms
			mvp = display * shroomTransform[i];
			glUniform3f(locColor, 0.61f, 0.18f, 0.18f);
			glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
			mushroom.draw();
		}

		// Drawing Suzanne if in third person
		if (camera.m_mode == Camera::Mode::THIRD_PERSON) {
			model = glm::translate(
				glm::mat4(1.0f),
				playerPos - glm::vec3(0., playerPos.y + 1, 0.)
			);
			model = glm::scale(
				model,
				glm::vec3(0.5f)
			);
			model = glm::rotate(
				model,
				glm::radians(-playerOrientation.x + 180.f),
				glm::vec3(0., 1., 0.)
			);
			mvp = display * model;
			glUniform3f(locColor, 0.71f, 0.31f, 0.74f);
			glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
			suzanne.draw();
		}

		// Disables Depth test for HUD
		// TODO: Ask if ok?
		glDisable(GL_DEPTH_TEST);
		// Drawing HUD
		float winWidth = w.getWidth();
		float winHeight = w.getHeight();
		model = glm::translate(
			glm::mat4(1.0f),
			glm::vec3((200. - winWidth) / winWidth, (200. - winHeight)/winHeight, 0.f)
		);
		model = glm::scale(
			model,
			glm::vec3(100.f / w.getWidth(), 100.f / w.getHeight(), 1.f)
		);
		glUniform3f(locColor, 1.f, 1.f, 0.f);
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(model));
		hud.draw(GL_TRIANGLES, 6);

		w.swap();
		w.pollEvent();
		isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
	}

	return 0;
}

void groupInstanciation(glm::mat4* treeTransform, glm::mat4* rockTransform, glm::mat4* shroomTransform) {
	glm::mat4 groupsTransform[N_GROUPS];
	for (int i = 0; i < N_GROUPS; ++i) {
		// Group transform
		float x, z;
		getGroupRandomPos(i, N_ROWS, x, z);
		float groupScale = rand01() * 0.6 + 0.7;
		float groupRotation = rand01() * 360.f;
		glm::mat4 model = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(x, -1., z)
		);
		model = glm::scale(
			model,
			glm::vec3(groupScale)
		);
		model = glm::rotate(
			model,
			glm::radians(groupRotation),
			glm::vec3(0., 1., 0.)
		);
		groupsTransform[i] = model;

		// Tree transform
		float treeScale = rand01() * 0.6 + 0.7;
		treeTransform[i] = glm::scale(
			groupsTransform[i],
			glm::vec3(treeScale)
		);

		// Rock Transform
		float rockRotation = rand01() * 360.f;
		float rockDistance = rand01() + 1.f;
		model = glm::rotate(
			groupsTransform[i],
			glm::radians(rockRotation),
			glm::vec3(0., 1., 0.)
		);
		model = glm::translate(
			model,
			glm::vec3(0., 0.2, rockDistance)
		);
		model = glm::scale(
			model,
			glm::vec3(0.3)
		);
		rockTransform[i] = model;

		// Scaling already applied in tree transform,
		model = glm::translate(
			treeTransform[i],
			glm::vec3(0.3, 0.0, 0.3)
		);
		// We still rescale because otherwise the mushroom is too big
		shroomTransform[i] = glm::scale(
			model,
			glm::vec3(0.1)
		);
	}
}

void checkGLError(int line) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "Line "<< line << ": ";
		switch (error) {
			case GL_INVALID_ENUM:
				std::cerr << "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				std::cerr << "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				std::cerr << "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				std::cerr << "GL_OUT_OF_MEMORY";
				break;
			case GL_STACK_UNDERFLOW:
				std::cerr << "GL_STACK_UNDERFLOW";
				break;
			case GL_STACK_OVERFLOW:
				std::cerr << "GL_STACK_OVERFLOW";
				break;
			default:
				std::cerr << "Unknown gl error occured!";
				break;
		}
		std::cerr << std::endl;
	}
}

void printGLInfo() {
	std::cout << "OpenGL info:" << std::endl;
	std::cout << "    Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

std::string readFile(const char* path) {
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
