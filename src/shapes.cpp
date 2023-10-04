#include "shapes.hpp"

BasicShapeElements::BasicShapeElements() {
	// Creates objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
}

BasicShapeElements::~BasicShapeElements() {
	glUseProgram(0);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeElements::setData(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize) {
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
	glEnableVertexArrayAttrib(m_vao, index);

	glBindVertexArray(0);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BasicShapeElements::drawTexture(GLenum mode, GLsizei count, Texture2D& texture) {
	glBindVertexArray(m_vao);
	texture.use();
	glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
	texture.unuse();
	glBindVertexArray(0);
}
