#include "shapes.hpp"

BasicShapeElements::BasicShapeElements() {
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

	glEnableVertexArrayAttrib(m_vao, index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);

	glBindVertexArray(0);
}

void BasicShapeElements::enablePosTex(ShaderProgram& s) {
	GLint locPos = s.getAttribLoc("pos");
	GLint locTex = s.getAttribLoc("inTexCoord");
	this->enableAttribute(locPos, 3, 5 * sizeof(GLfloat), 0);
	this->enableAttribute(locTex, 2, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));
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

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize) {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

BasicShapeArrays::~BasicShapeArrays() {
	glUseProgram(0);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexArrayAttrib(m_vao, index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
	glBindVertexArray(0);
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawArrays(mode, 0, count);
	glBindVertexArray(0);
}
