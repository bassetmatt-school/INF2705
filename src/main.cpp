#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

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
		std::string fragmentShaderCode = readFile("shaders/model.fs.glsl");
		std::string vertexShaderCode = readFile("shaders/model.vs.glsl");
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

	// Shader attributes
	GLint locMVP;
	{
		GLint locVertex = modelShader.getAttribLoc("scenePosition");
		GLint locColor = modelShader.getAttribLoc("color");
		locMVP = modelShader.getUniformLoc("pvmMatrix");
		cube.enableAttribute(locVertex, 3, 24, 0);
		cube.enableAttribute(locColor, 3, 24, 12);
	}

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
		glm::mat4 proj = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 10.0f);

		// View matrix
		// At (0, 0.5, 2), looking at the origin, with y as up vector
		glm::mat4 view = glm::lookAt(
			glm::vec3(0., 0.5, 2.),
			glm::vec3(0., 0., 0.),
			glm::vec3(0., 1., 0.)
		);

		// Model matrix
		// Rotates around this axis (normalized), with an angle of angleDeg
		glm::vec3 axis(0.1, 1., 0.1);
		glm::mat4 model = glm::rotate(
			glm::mat4(1.0f), // Base matrix, identity here
			glm::radians(angleDeg),
			glm::normalize(axis)
		);

		// MVP matrix assembly
		glm::mat4 mvp = proj * view * model;

		// Send matrix to shader
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));


		// Drawing

		cube.draw(GL_TRIANGLES, 36);
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
