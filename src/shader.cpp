#include "shader.h"

#include <iostream>
#include <cstdarg>

GLuint compile_shader(const char* path, GLenum shader_type)
{
    FILE* shader_file = fopen(path, "rb");
    if(shader_file == NULL)
    {
		std::cerr << "Shader source file at " << path << " not found\n";
		std::exit(-1);
    }
    fseek(shader_file, 0, SEEK_END);
    const long file_size = ftell(shader_file);
	rewind(shader_file);

    char* shader_source = new char[file_size + 1];
	fread(shader_source, file_size, 1, shader_file);
	fclose(shader_file);
	shader_source[file_size] = '\0';

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char *const*)&shader_source, NULL);
    glCompileShader(shader);
	delete[] shader_source;

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cerr << "Could not compile shader at " << path << ":\n" << info_log << "\n";
		std::exit(-1);
    }

    return shader;
}

GLuint link_shader_program(const unsigned int shader_count, ...)
{
    /* links program AND deletes all shader objects */

    va_list shader_list;
    va_start(shader_list, shader_count);

    va_list shader_clean_list;
    va_copy(shader_clean_list, shader_list);

    GLuint shader_program = glCreateProgram();
    for(unsigned int i = 0; i < shader_count; ++i)
    {
        GLuint shader = va_arg(shader_list, GLuint);
        glAttachShader(shader_program, shader);
    }
    glLinkProgram(shader_program);
    va_end(shader_list);

    for(unsigned int i = 0; i < shader_count; ++i)
    {
        GLuint shader = va_arg(shader_clean_list, GLuint);
    	glDetachShader(shader_program, shader);
        glDeleteShader(shader);
    }
    va_end(shader_clean_list);

    return shader_program;
}

void destroy_shader_program(GLuint shader_program)
{
	glDeleteProgram(shader_program);
}
