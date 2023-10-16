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

	int format = (nChannels == 3) ? GL_RGB : GL_RGBA;

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	// Wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	// Sampling methods
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_id);
}

void Texture2D::enableMipmap() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::use() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::unuse() {
	glBindTexture(GL_TEXTURE_2D, 0);
}


TextureCubeMap::TextureCubeMap(const char** pathes) {
	const size_t N_TEXTURES = 6;
	unsigned char* datas[N_TEXTURES];
	int widths[N_TEXTURES];
	int heights[N_TEXTURES];
	int nChannels[N_TEXTURES];
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < N_TEXTURES; i++) {
		datas[i] = stbi_load(pathes[i], &widths[i], &heights[i], &nChannels[i], 0);
		if (datas[i] == NULL)
			std::cout << "Error loading texture \"" << pathes[i] << "\": " << stbi_failure_reason() << std::endl;
	}

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	for (unsigned int i = 0; i < N_TEXTURES; i++) {
		int format = (nChannels[i] == 3) ? GL_RGB : GL_RGBA;
		int target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glTexImage2D(target, 0, format, widths[i], heights[i], 0, format, GL_UNSIGNED_BYTE, datas[i]);
		stbi_image_free(datas[i]);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

TextureCubeMap::~TextureCubeMap() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_id);
}

void TextureCubeMap::use() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}
