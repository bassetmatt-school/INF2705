#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "shader_program.hpp"
#include "vertices_data.hpp"
#include "shapes.hpp"


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__LINE__)
void checkGLError(int line);

void changeRGB(GLfloat* color);
void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy);

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

	// Basic shader
	ShaderProgram basic;
	{
		std::string fragmentShaderCode = readFile("shaders/basic.fs.glsl");
		std::string vertexShaderCode = readFile("shaders/basic.vs.glsl");
		Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode.c_str());
		Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());
		basic.attachShader(vertexShader);
		basic.attachShader(fragmentShader);
		basic.link();
	}

	// Color shader
	ShaderProgram color;
	{
		std::string fragmentShaderCode = readFile("shaders/color.fs.glsl");
		std::string vertexShaderCode = readFile("shaders/color.vs.glsl");
		Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode.c_str());
		Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());
		color.attachShader(vertexShader);
		color.attachShader(fragmentShader);
		color.link();
	}

	// Transform shader
	ShaderProgram transform;
	{
		std::string fragmentShaderCode = readFile("shaders/transform.fs.glsl");
		std::string vertexShaderCode = readFile("shaders/transform.vs.glsl");
		Shader vertexShader(GL_VERTEX_SHADER, vertexShaderCode.c_str());
		Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());
		transform.attachShader(vertexShader);
		transform.attachShader(fragmentShader);
		transform.link();
	}

	// Variables pour la mise à jour, ne pas modifier.
	float cx = 0, cy = 0;
	float dx = 0.019;
	float dy = 0.0128;

	float angleDeg = 0.0f;

	// Tableau non constant de la couleur
	GLfloat onlyColorTriVertices[] ={
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};

	// Create shapes
	GLint locVertex, locColor, locMVP;
	locVertex = basic.getAttribLoc("aPos");
	// Shape 0: Simple triangle
	BasicShapeArrays triangle_unicolor(triVertices, sizeof(triVertices));
	triangle_unicolor.enableAttribute(locVertex, 3, 12, 0);

	// Shape 1: Simple square
	BasicShapeArrays square_unicolor(squareVertices, sizeof(squareVertices));
	square_unicolor.enableAttribute(locVertex, 3, 12, 0);

	// Second shader
	locVertex = color.getAttribLoc("aPos");
	locColor = color.getAttribLoc("color");
	// Shape 2: RGB Triangle
	BasicShapeArrays triangle_rgb(colorTriVertices, sizeof(colorTriVertices));
	triangle_rgb.enableAttribute(locVertex, 3, 24, 0);
	triangle_rgb.enableAttribute(locColor, 3, 24, 12);

	// Shape 3: RGB Square
	BasicShapeArrays square_rgb(colorSquareVertices, sizeof(colorSquareVertices));
	square_rgb.enableAttribute(locVertex, 3, 24, 0);
	square_rgb.enableAttribute(locColor, 3, 24, 12);

	// Shape 4: Triangle that moves and changes color
	BasicShapeMultipleArrays triangle_updated(
		triVertices,
		sizeof(triVertices),
		onlyColorTriVertices,
		sizeof(onlyColorTriVertices)
	);
	triangle_updated.enablePosAttribute(locVertex, 3, 12, 0);
	triangle_updated.enableColorAttribute(locColor, 3, 12, 0);

	// Shape 5: Square with less vertices specified
	BasicShapeElements square_reduced(
		colorSquareVerticesReduced,
		sizeof(colorSquareVerticesReduced),
		indexes,
		sizeof(indexes)
	);
	square_reduced.enableAttribute(locVertex, 3, 24, 0);
	square_reduced.enableAttribute(locColor, 3, 24, 12);

	// Third shader
	locVertex = transform.getAttribLoc("scenePosition");
	locColor = transform.getAttribLoc("color");
	locMVP = transform.getUniformLoc("pvmMatrix");
	// Part 2 cube
	BasicShapeElements cube(
		cubeVertices,
		sizeof(cubeVertices),
		cubeIndexes,
		sizeof(cubeIndexes)
	);
	cube.enableAttribute(locVertex, 3, 24, 0);
	cube.enableAttribute(locColor, 3, 24, 12);

	// Background color
	glm::vec4 clearColor(0.11f, 0.12f, 0.13f, 1.0f);

	// Enables depth test
	glEnable(GL_DEPTH_TEST);

	int selectShape = 0;
	bool isRunning = true;
	while (isRunning) {
		// Background Color
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		if (w.shouldResize())
			glViewport(0, 0, w.getWidth(), w.getHeight());

		// Clears buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (w.getKey(Window::Key::T)) {
			++selectShape %= 7;
			std::cout << "Selected shape: " << selectShape << std::endl;
		}

		// Position udpate for case 4
		GLfloat* posPtr = triangle_updated.mapPosData();
		changePos(posPtr, cx, cy, dx, dy);
		triangle_updated.unmapPosData();
		// Color update for case 4
		changeRGB(&onlyColorTriVertices[0]);
		changeRGB(&onlyColorTriVertices[3]);
		changeRGB(&onlyColorTriVertices[6]);
		triangle_updated.updateColorData(onlyColorTriVertices, sizeof(onlyColorTriVertices));


		// Shader selection
		switch (selectShape) {
			case 0:
			case 1:
				basic.use();
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				color.use();
				break;
			case 6:
				transform.use();
				break;
			default:
				break;
		}

		// MVP matrix update for case 6
		if (selectShape == 6) {
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
		}

		// Drawing
		switch (selectShape) {
			case 0:
				triangle_unicolor.draw(GL_TRIANGLES, 3);
				break;
			case 1:
				square_unicolor.draw(GL_TRIANGLES, 6);
				break;
			case 2:
				triangle_rgb.draw(GL_TRIANGLES, 3);
				break;
			case 3:
				square_rgb.draw(GL_TRIANGLES, 6);
				break;
			case 4:
				triangle_updated.draw(GL_TRIANGLES, 3);
				break;
			case 5:
				square_reduced.draw(GL_TRIANGLES, 6);
				break;
			case 6:
				cube.draw(GL_TRIANGLES, 36);
				break;
			default:
				break;
		}

		w.swap();
		w.pollEvent();
		isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
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


void changeRGB(GLfloat* color) {
	unsigned char r = color[0]*255;
	unsigned char g = color[1]*255;
	unsigned char b = color[2]*255;

	if (r > 0 && b == 0) {
		r--;
		g++;
	}
	if (g > 0 && r == 0) {
		g--;
		b++;
	}
	if (b > 0 && g == 0) {
		r++;
		b--;
	}
	color[0] = r/255.0f;
	color[1] = g/255.0f;
	color[2] = b/255.0f;
}

void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy) {
	if ((cx < -1 && dx < 0) || (cx > 1 && dx > 0))
		dx = -dx;
	pos[0] += dx;
	pos[3] += dx;
	pos[6] += dx;
	cx += dx;
	if ((cy < -1 && dy < 0) || (cy > 1 && dy > 0))
		dy = -dy;
	pos[1] += dy;
	pos[4] += dy;
	pos[7] += dy;
	cy += dy;
}


std::string readFile(const char* path) {
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
