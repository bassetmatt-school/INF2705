#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>

class Shader {
	public:
	Shader(GLenum type, const char* code);
	~Shader();

	GLuint id();

	private:
	void checkError();

	private:
	GLuint m_id;
};

class ShaderProgram {
	public:
	ShaderProgram();
	~ShaderProgram();

	void init(const char* vsPath, const char* fsPath);
	void init(const char* vsPath, const char* gsPath, const char* fsPath);

	void use();
	void attachShader(Shader& s);
	void link();

	GLint getAttribLoc(const char* name);
	GLint getUniformLoc(const char* name);
	void setUniformBlockBinding(const char* name, GLuint bindingIndex);

	private:
	void checkError();

	private:
	GLuint m_id;
};


#endif // SHADER_PROGRAM_H
