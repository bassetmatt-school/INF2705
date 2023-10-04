#include "textures.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include <iostream>

Texture2D::Texture2D(const char* path, GLenum wrapMode) {
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	if (data == NULL)
		std::cout << "Error loading texture \"" << path << "\": " << stbi_failure_reason() << std::endl;

	// TODO: Chargement	de la texture.
	glGenTextures(1, &m_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_id);
}

void Texture2D::enableMipmap() {
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::use() {
	// TODO: Utilise la texture
	glBindTexture(GL_TEXTURE_2D, m_id);
	glActiveTexture(GL_TEXTURE0);
}

void Texture2D::unuse() {
	glActiveTexture(0);
}


TextureCubeMap::TextureCubeMap(const char** pathes) {
	const size_t N_TEXTURES = 6;
	unsigned char* datas[N_TEXTURES];
	int widths[N_TEXTURES];
	int heights[N_TEXTURES];
	int nChannels[N_TEXTURES];
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < 6; i++) {
		datas[i] = stbi_load(pathes[i], &widths[i], &heights[i], &nChannels[i], 0);
		if (datas[i] == NULL)
			std::cout << "Error loading texture \"" << pathes[i] << "\": " << stbi_failure_reason() << std::endl;
	}


	// TODO: Chargement des textures du cubemap.


	for (unsigned int i = 0; i < 6; i++) {
		stbi_image_free(datas[i]);
	}


}

TextureCubeMap::~TextureCubeMap() {
	// TODO: Supprimer la mémoire de l'objet
}

void TextureCubeMap::use() {
	// TODO: Utilise la texture du cubemap
}
