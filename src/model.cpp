#include "model.hpp"

#include "obj_loader.hpp"
#include <iostream>

Model::Model(const char* path) {

}

void Model::loadObj(const char* path, std::vector<GLfloat>& pos, std::vector<GLuint>& indices) {
	// TODO Complete
	// objl::Loader loader;
	// bool loadout = loader.LoadFile(path);
	// if (!loadout) {
	// 	std::cout << "Unable to load model " << path << std::endl;
	// 	return;
	// }

	// for (size_t i = 0; i < loader.LoadedVertices.size(); i++) {
	// 	objl::Vector3 p = loader.LoadedVertices[i].Position;
	// 	pos.push_back(p.X);
	// 	pos.push_back(p.Y);
	// 	pos.push_back(p.Z);
	// }
	// indices = loader.LoadedIndices;
}

void Model::draw() {

}
