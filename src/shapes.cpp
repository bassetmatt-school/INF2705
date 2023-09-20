#include "shapes.h"

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize) {
	// Creates objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// VBO data bound and loaded with VAO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

	// Unselects VAO
	glBindVertexArray(0);
}

BasicShapeArrays::~BasicShapeArrays() {
	// Unselects shader
	glUseProgram(0);
	// Deletes data
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// Enables the attribute
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
	// Using instead `glEnableVertexArrayAttrib` of `glEnableVertexAttribArray`
	// They have the same behaviour (?) but with this one, there is an extra safety by
	// specifying the VAO to use
	glEnableVertexArrayAttrib(m_vao, index);
	// Unselects VAO
	glBindVertexArray(0);

}

void BasicShapeArrays::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawArrays(mode, 0, count);
	glBindVertexArray(0);
}



BasicShapeMultipleArrays::BasicShapeMultipleArrays(const GLfloat* pos, GLsizeiptr posByteSize, const GLfloat* color, GLsizeiptr colorByteSize) {
	// Creates objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_posVbo);
	glGenBuffers(1, &m_colorVbo);

	// Selects VAO
	glBindVertexArray(m_vao);
	// position VBO allocating and initializing
	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, posByteSize, pos, GL_STATIC_DRAW);
	// color VBO allocating and initializing
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, colorByteSize, color, GL_STATIC_DRAW);

	// Unselects VAO
	glBindVertexArray(0);
}

BasicShapeMultipleArrays::~BasicShapeMultipleArrays() {
	// Unselects shader
	glUseProgram(0);
	// Deletes data
	glDeleteBuffers(1, &m_posVbo);
	glDeleteBuffers(1, &m_colorVbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeMultipleArrays::enablePosAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo); // position
	// Enables the attribute
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
	glEnableVertexArrayAttrib(m_vao, index);
	// Unselects VAO
	glBindVertexArray(0);
}

void BasicShapeMultipleArrays::enableColorAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo); // color
	// Enables the attribute
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
	glEnableVertexArrayAttrib(m_vao, index);
	// Unselects VAO
	glBindVertexArray(0);
}

void BasicShapeMultipleArrays::updateColorData(const GLfloat* color, GLsizeiptr colorByteSize) {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colorByteSize, color);
	// Unselects VAO
	glBindVertexArray(0);
}

GLfloat* BasicShapeMultipleArrays::mapPosData() {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	// Maps the buffer
	GLfloat* pos = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	// Unselects VAO
	glBindVertexArray(0);
	return pos;
}

void BasicShapeMultipleArrays::unmapPosData() {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	// Unmaps the buffer
	glUnmapBuffer(GL_ARRAY_BUFFER);
	// Unselects VAO
	glBindVertexArray(0);
}

void BasicShapeMultipleArrays::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawArrays(mode, 0, count);
	glBindVertexArray(0);
}



BasicShapeElements::BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLubyte* indexes, GLsizeiptr indexesByteSize) {
	// Creates objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	// Selects VAO
	glBindVertexArray(m_vao);
	// Shape VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
	// Connection Array VBO (EBO), using the correct enum variant
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);

	// Unselects VAO
	glBindVertexArray(0);
}

BasicShapeElements::~BasicShapeElements() {
	// Unselects shader
	glUseProgram(0);
	// Deletes data
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// Enables the attribute
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
	glEnableVertexArrayAttrib(m_vao, index);
	// Unselects VAO
	glBindVertexArray(0);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawElements(mode, count, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}
