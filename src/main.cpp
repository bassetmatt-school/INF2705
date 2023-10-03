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
#include "vertices_data.hpp"
#include "window.hpp"


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__LINE__)
void checkGLError(int line);

std::string readFile(const char* path);


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
	BasicShapeElements cube;
	cube.setData(
		cubeVertices,
		sizeof(cubeVertices),
		cubeIndexes,
		sizeof(cubeIndexes)
	);

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

	Model suzanne("../models/suzanne.obj");
	Model tree("../models/tree.obj");
	Model mushroom("../models/mushroom.obj");
	Model rock("../models/rock.obj");

	// Groups
	const int N_ROWS = 7;
	const int N_GROUPS = N_ROWS * N_ROWS;

	glm::mat4 groupsTransform[N_GROUPS];

	glm::mat4 treeTransform[N_GROUPS];
	glm::mat4 rockTransform[N_GROUPS];
	glm::mat4 shroomTransform[N_GROUPS];
	// Shader attributes
	GLint locMVP;
	{
		GLint locVertex = modelShader.getAttribLoc("scenePosition");
		GLint locColor = modelShader.getAttribLoc("color");
		locMVP = modelShader.getUniformLoc("pvmMatrix");
		cube.enableAttribute(locVertex, 3, 24, 0);
		cube.enableAttribute(locColor, 3, 24, 12);
		ground.enableAttribute(locVertex, 3, 24, 0);
		ground.enableAttribute(locColor, 3, 24, 12);
		river.enableAttribute(locVertex, 3, 24, 0);
		river.enableAttribute(locColor, 3, 24, 12);
	}

	// Camera
	glm::vec3 playerPos(0.);
	glm::vec2 playerOrientation(0.);
	Camera camera(playerPos, playerOrientation);

	// Background color, sort of dark gray
	glm::vec4 clearColor(0.11f, 0.12f, 0.13f, 1.0f);

	// Enables depth test
	glEnable(GL_DEPTH_TEST);

	bool isRunning = true;
	int mouseX, mouseY;
	while (isRunning) {
		// Background Color
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		if (w.shouldResize())
			glViewport(0, 0, w.getWidth(), w.getHeight());

		// Clears buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Projection matrix
		// FOV: 70°, near plane at 0.1, far plane at 10
		float aspect = w.getWidth() / (float) w.getHeight();
		glm::mat4 proj = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 100.0f);

		w.getMouseMotion(mouseX, mouseY);
		playerOrientation.x += mouseX * 0.1;
		playerOrientation.y += mouseY * 0.1;
		// Prevents camera from going upside down
		playerOrientation.y = glm::clamp(playerOrientation.y, -90.f, 90.f);
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


		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		// MVP matrix assembly
		glm::mat4 mvp = display * model;
		// Send matrix to shader
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));

		// Drawing
		ground.draw(GL_TRIANGLES, 6);
		river.draw(GL_TRIANGLES, 6);

		model = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(10., 0., 10.)
		);
		model *= glm::scale(
			glm::mat4(1.0f),
			glm::vec3(0.5f)
		);
		// MVP matrix assembly
		mvp = display * model;

		// Send matrix to shader
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
		suzanne.draw();

		if (camera.m_mode == Camera::Mode::THIRD_PERSON) {
			model = glm::translate(
				glm::mat4(1.0f),
				playerPos - glm::vec3(0., 1, 0.)
			);
			model *= glm::scale(
				glm::mat4(1.0f),
				glm::vec3(0.5f)
			);
			model *= glm::rotate(
				glm::mat4(1.0f),
				glm::radians(-playerOrientation.x + 180.f),
				glm::vec3(0., 1., 0.)
			);
			// MVP matrix assembly
			mvp = display * model;

			// Send matrix to shader
			glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
			suzanne.draw();
		}

		w.swap();
		w.pollEvent();
		isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
	}

	return 0;
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
