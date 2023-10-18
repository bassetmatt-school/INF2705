#ifndef RESOURCES_H
#define RESOURCES_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader_program.hpp"
#include "shapes.hpp"
#include "model.hpp"
#include "textures.hpp"

class Resources {
	public:
	Resources();

	// #############
	// SHADERS
	// #############
	ShaderProgram model;
	GLint mvpLocationModel;
	// ---
	ShaderProgram phong;
	GLint mvpLocationPhong;
	GLint modelViewLocationPhong;
	GLint viewLocationPhong;
	GLint normalLocationPhong;
	// ---
	ShaderProgram gouraud;
	GLint mvpLocationGouraud;
	GLint modelViewLocationGouraud;
	GLint viewLocationGouraud;
	GLint normalLocationGouraud;
	// ---
	ShaderProgram flat;
	GLint mvpLocationFlat;
	GLint modelViewLocationFlat;
	GLint viewLocationFlat;
	GLint normalLocationFlat;
	// ---
	ShaderProgram simple;
	GLint mvpLocationSimple;
	GLint colorLocationSimple;
	// ---
	ShaderProgram skyboxShader;
	GLint mvpLocationSky;
	// #############
	// SIMPLE GEOMETRY MESHES
	// #############
	BasicShapeElements ground;
	BasicShapeArrays skybox;
	// #############
	// MODELS
	// #############
	Model glass;
	Model suzanne;
	Model sphere;
	Model cube;
	Model spotlight;
	Model rock;
	// #############
	// TEXTURES
	// #############
	Texture2D glassTexture;
	Texture2D suzanneTexture;
	Texture2D whiteTexture;
	Texture2D groundTexture;
	Texture2D rockTexture;
	TextureCubeMap skyboxTexture;
};

#endif // RESOURCES_H
