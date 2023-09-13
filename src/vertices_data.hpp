#ifndef VERTICES_DATA_H
#define VERTICES_DATA_H

#include <GL/glew.h>

const GLfloat triVertices[] ={
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

const GLfloat squareVertices[] ={
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,

	-0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

// const GLfloat colorTriVertices[] ={
// 	1.0f, 0.0f, 0.0f,
// 	0.0f, 1.0f, 0.0f,
// 	0.0f, 0.0f, 1.0f,
// };

const GLfloat colorTriVertices[] ={
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

// const GLfloat colorSquareVertices[] ={
// 	1.0f, 0.0f, 0.0f,
// 	0.0f, 1.0f, 0.0f,
// 	0.0f, 0.0f, 1.0f,

// 	0.0f, 0.0f, 1.0f,
// 	0.0f, 1.0f, 0.0f,
// 	1.0f, 0.0f, 0.0f,
// };

const GLfloat colorSquareVertices[] ={
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
};


// const GLfloat colorSquareVerticesReduced[] ={
// 	// TODO Partie 1: Définir les coordonnées d'un carré
// 	// fait de deux triangles avec les attributs de
// 	// position et couleur entrelacés en fonction de
// 	// l'entré du vertex shader. Il devrait y avoir
// 	// moins de données dans ce tableau.
// };
// const GLubyte indexes[] ={
// 	// TODO Partie 1: Définir les indexes pour générer les
// 	// primitives du carré en réutilisant certaines vertices.
// };


// Partie 2: On vous donne les vertices et indexes du cube.
// Inspirez-vous en pour les tableaux au-dessus au besoin.
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

const GLubyte cubeIndexes[] ={
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

#endif // VERTICES_DATA_H
