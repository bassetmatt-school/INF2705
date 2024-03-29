#ifndef VERTICES_DATA_H
#define VERTICES_DATA_H

#include <GL/glew.h>

const GLfloat cubeVertices[] ={
	// +z
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	 // -z
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	 // +x
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
	 // -x
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	// +y
  -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	// -y
  -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
};

const GLuint cubeIndexes[] ={
	  0,  1,  2,
	  1,  3,  2,

	  4,  5,  6,
	  5,  7,  6,

	  8,  9, 10,
	  9, 11, 10,

	 12, 13, 14,
	 13, 15, 14,

	 16, 17, 18,
	 17, 19, 18,

	 20, 21, 22,
	 21, 23, 22,
};


#define GROUND_SIZE 30.0f
GLfloat groundVertices[] ={
	 -GROUND_SIZE, -1.0f, GROUND_SIZE, 0.0, 10.0,
	 GROUND_SIZE, -1.0f, GROUND_SIZE, 10.0, 10.0,
	 GROUND_SIZE, -1.0f, -GROUND_SIZE, 10.0, 0.0,
	 -GROUND_SIZE, -1.0f, -GROUND_SIZE, 0.0, 0.0
};

GLuint groundIndexes[] ={
	 0, 1, 2,
	 0, 2, 3
};

#define RIVER_WIDTH_X 20.0f
GLfloat riverVertices[] ={
	 -RIVER_WIDTH_X - GROUND_SIZE, -1.0f, GROUND_SIZE, 0.0, 1.0,
	 -GROUND_SIZE, -1.0f, GROUND_SIZE, 1.0, 1.0,
	 -GROUND_SIZE, -1.0f, -GROUND_SIZE, 1.0, 0.0,
	 -RIVER_WIDTH_X - GROUND_SIZE, -1.0f, -GROUND_SIZE, 0.0, 0.0
};

GLfloat quad2dVertices[] ={
	 -0.5f, -0.5f, 0, 0.0, 0.0,
	 0.5f, -0.5f, 0, 1.0, 0.0,
	 0.5f, 0.5f, 0, 1.0, 1.0,
	 -0.5f, 0.5f, 0, 0.0, 1.0
};

GLuint quad2dIndexes[] ={
	 0, 1, 2,
	 0, 2, 3
};

GLfloat skyboxVertices[] ={
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

GLfloat grassPosition[] ={
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
GLfloat grassTexCoords[] ={
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
