#include "shapes.hpp"

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize) {
	// Creates objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// VBO data bound with VAO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

	// Unselects VAO
	glBindVertexArray(0);
}

BasicShapeArrays::~BasicShapeArrays() {
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	// Selects VAO and VBO
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// Enables the attribute
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset);
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
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), colorByteSize, color);
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
	// TODO Partie 1: Générer et bind le vao de la forme.

	// TODO Partie 1: Générer et bind le vbo et ebo de la forme.
	// Allouer l'espace nécessaire dans le mode approprié
	// et envoyer les données au gpu.
}

BasicShapeElements::~BasicShapeElements() {
	// TODO Partie 1: Supprimer la mémoire de l'objet.
	// Assurez-vous que les ressources ne soient pas utilisées
	// pendant la suppression.
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
	// TODO Partie 1: Activer un attribut et l'attacher correctement au state du vao.
}

void BasicShapeElements::draw(GLenum mode, GLsizei count) {
	// TODO Partie 1: Dessiner la forme avec le ebo.
}
