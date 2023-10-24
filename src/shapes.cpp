#include "shapes.hpp"

BasicShapeElements::BasicShapeElements() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

BasicShapeElements::BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);
}

BasicShapeElements::~BasicShapeElements() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*) (offset * sizeof(GLfloat)));
}

void BasicShapeElements::enablePosTex(ShaderProgram& s) {
	GLint locPos = s.getAttribLoc("inPos");
	GLint locTex = s.getAttribLoc("inTexCoord");
	this->enableAttribute(locPos, 3, 5, 0);
	this->enableAttribute(locTex, 2, 5, 3);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
}

void BasicShapeElements::drawTexture(GLenum mode, GLsizei count, Texture2D& texture) {
	glBindVertexArray(m_vao);
	texture.use();
	glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
	texture.unuse();
}

BasicShapeArrays::BasicShapeArrays() {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
}

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
}

BasicShapeArrays::~BasicShapeArrays() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeArrays::setData(const GLfloat* data, GLsizeiptr byteSize) {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*) (offset * sizeof(GLfloat)));
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawArrays(mode, 0, count);
}
