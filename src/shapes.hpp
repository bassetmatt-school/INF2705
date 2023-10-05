#ifndef SHAPES_H
#define SHAPES_H

#include <GL/glew.h>
#include "shader_program.hpp"
#include "textures.hpp"

class BasicShapeElements {
	public:
	BasicShapeElements();
	~BasicShapeElements();

	void setData(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize);
	void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);
	void enablePosTex(ShaderProgram& s);
	void draw(GLenum mode, GLsizei count);
	void drawTexture(GLenum mode, GLsizei count, Texture2D& texture);

	private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
};

class BasicShapeArrays {
	public:
	BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize);
	~BasicShapeArrays();

	void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);
	void draw(GLenum mode, GLsizei count);

	private:
	GLuint m_vao;
	GLuint m_vbo;
};

#endif // SHAPES_H
