#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "window.hpp"
#include "shader_program.hpp"
#include "vertices_data.hpp"
#include "shapes.hpp"


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
	float angleDeg = 0.0f;


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
	}

	// Camera
	glm::vec3 playerPos(0., 1., 0.);
	glm::vec2 playerOrientation(0.);
	Camera camera(playerPos, playerOrientation);

	// Background color, sort of dark gray
	glm::vec4 clearColor(0.11f, 0.12f, 0.13f, 1.0f);

	// Enables depth test
	glEnable(GL_DEPTH_TEST);

	bool isRunning = true;
	while (isRunning) {
		// Background Color
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		if (w.shouldResize())
			glViewport(0, 0, w.getWidth(), w.getHeight());

		// Clears buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.use();

		// MVP matrix update
		angleDeg += 0.5f;

		// Projection matrix
		// FOV: 70°, near plane at 0.1, far plane at 10
		float aspect = w.getWidth() / (float) w.getHeight();
		glm::mat4 proj = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 100.0f);
		int x, y;
		w.getMouseMotion(x, y);
		playerOrientation.x += x * 0.1;
		playerOrientation.y += y * 0.1;
		float theta = glm::radians(playerOrientation.x);
		glm::vec3 forward(glm::sin(theta), 0., -glm::cos(theta));
		glm::vec3 right = glm::cross(forward, glm::vec3(0., 1., 0.));
		if (w.getKeyHold(Window::Key::W)) playerPos += 0.08f * forward;
		if (w.getKeyHold(Window::Key::S)) playerPos -= 0.08f * forward;
		if (w.getKeyHold(Window::Key::A)) playerPos -= 0.08f * right;
		if (w.getKeyHold(Window::Key::D)) playerPos += 0.08f * right;
		if (w.getKeyHold(Window::Key::Q)) playerPos.y += 0.08f;
		if (w.getKeyHold(Window::Key::E)) playerPos.y -= 0.08f;

		// View matrix
		// At (0, 0.5, 2), looking at the origin, with y as up vector
		glm::mat4 view = camera.getFirstPersonViewMatrix();

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);

		// MVP matrix assembly
		glm::mat4 mvp = proj * view * model;

		// Send matrix to shader
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));


		// Drawing

		cube.draw(GL_TRIANGLES, 36);
		ground.draw(GL_TRIANGLES, 6);
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
