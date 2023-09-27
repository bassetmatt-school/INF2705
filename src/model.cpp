#include "model.hpp"

#include "obj_loader.hpp"
#include <iostream>

Model::Model(const char* path) {
	std::vector<GLfloat> pos;
	std::vector<GLuint> indices;
	loadObj(path, pos, indices);
	m_count = indices.size();
	m_shape.setData(
		pos.data(),
		pos.size() * sizeof(GLfloat),
		indices.data(),
		indices.size() * sizeof(GLuint)
	);
	m_shape.enableAttribute(0, 3, 0, 0);
}

void Model::loadObj(const char* path, std::vector<GLfloat>& pos, std::vector<GLuint>& indices) {
	objl::Loader loader;
	bool loadout = loader.LoadFile(path);
	if (!loadout) {
		std::cout << "Unable to load model " << path << std::endl;
		return;
	}

	for (size_t i = 0; i < loader.LoadedVertices.size(); i++) {
		objl::Vector3 p = loader.LoadedVertices[i].Position;
		pos.push_back(p.X);
		pos.push_back(p.Y);
		pos.push_back(p.Z);
		// TODO: Décommenter lors de la partie 2
		//objl::Vector2 t = loader.LoadedVertices[i].TextureCoordinate;
		//vertexData.push_back(t.X);
		//vertexData.push_back(t.Y);
	}
	indices = loader.LoadedIndices;
}

void Model::draw() {
	m_shape.draw(GL_TRIANGLES, m_count);
}
