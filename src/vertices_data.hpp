#ifndef VERTICES_DATA_H
#define VERTICES_DATA_H

#include <GL/glew.h>

const GLfloat groundVertices[] ={
	-30.f, -1.f, -30.f, 0.f, 0.f,
	-30.f, -1.f,  30.f, 0.f, 15.f,
	 30.f, -1.f, -30.f, 15.f, 0.f,
	 30.f, -1.f,  30.f, 15.f, 15.f,
};

const GLuint groundIndexes[] ={
	0,  1,  2,
	1,  3,  2,
};


const GLfloat riverVertices[] ={
	-30.f, -1.f, -50.f, 0.f, 0.f,
	-30.f, -1.f, -30.f, 0.f, 2.f,
	 30.f, -1.f, -50.f, 5.f, 0.f,
	 30.f, -1.f, -30.f, 5.f, 2.f,
};

const GLuint riverIndexes[] ={
	0,  1,  2,
	1,  3,  2,
};

const GLfloat hudVertices[] ={
	-1.f, -1.f, 0.f, 0.f, 0.f,
	 1.f, -1.f, 0.f, 1.f, 0.f,
	-1.f,  1.f, 0.f, 0.f, 1.f,
	 1.f,  1.f, 0.f, 1.f, 1.f,
};

const GLuint hudIndexes[] ={
	0,  1,  2,
	1,  3,  2,
};

const char* skyboxPaths[] ={
	"../textures/skybox/Daylight Box_Right.bmp",
	"../textures/skybox/Daylight Box_Left.bmp",
	"../textures/skybox/Daylight Box_Top.bmp",
	"../textures/skybox/Daylight Box_Bottom.bmp",
	"../textures/skybox/Daylight Box_Front.bmp",
	"../textures/skybox/Daylight Box_Back.bmp",
};

const GLfloat skyboxVertices[] ={
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

const GLfloat grassPosition[] ={
	0.433013f, 0.0f, -0.25f,
	-0.433013f, 0.75f, 0.25f,
	0.433013f, 0.75f, -0.25f,

	0.433013f,  0.0f,  -0.25f,
	-0.433013f, 0.0f,  0.25f,
	-0.433013f,  0.75f,  0.25f,

	0.0f     ,  0.0f, -0.5f,
	0.0f     ,  0.75f,  0.5f,
	0.0f     ,  0.0f,  0.5f,

	0.0f     ,  0.0f, -0.5f,
	0.0f     ,  0.75f, -0.5f,
	0.0f     ,  0.75f,  0.5f,

	-0.433013f,  0.0f, -0.25f,
	0.433013f,  0.75f,  0.25f,
	0.433013f,  0.0f,  0.25f,

	-0.433013f,  0.0f, -0.25f,
	-0.433013f,  0.75f, -0.25f,
	0.433013f,  0.75f,  0.25f,
};

const GLfloat grassTexCoords[] ={
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,


	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,


	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};


#endif // VERTICES_DATA_H
