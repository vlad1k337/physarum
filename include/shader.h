#ifndef __SHADER_H
#define __SHADER_H

#include "glad/gl.h"

GLuint compile_shader(const char* path, GLenum shader_type);

GLuint link_shader_program(const unsigned int shader_count, ...);
void   destroy_shader_program(GLuint shader_program);

inline void send_uniform_float(GLuint shader_program, const GLchar* uniform_name, GLfloat v0)
{
    GLint location = glGetUniformLocation(shader_program, uniform_name);
    glUniform1f(location, v0);
}

inline void send_uniform_vec2(GLuint shader_program, const GLchar* uniform_name, GLfloat v0, GLfloat v1)
{
    GLint location = glGetUniformLocation(shader_program, uniform_name);
    glUniform2f(location, v0, v1);
}

#endif
