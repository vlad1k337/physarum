#include "mold.h"

#include <cmath>
#include <cwchar>
#include <random>
#include "shader.h"

void TrailMap::create()
{
	create_texture();
	create_compute_program();
	create_shader_program();
	create_quad_vertex_array();
}

void TrailMap::create_texture()
{
	glCreateTextures(GL_TEXTURE_2D, 1, &image);
	glBindTextureUnit(0, image);
	glTextureParameteri(image, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(image, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(image, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(image, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureStorage2D(image, 1, GL_RGBA32F, map_width, map_height);
	glTextureSubImage2D(image, 0, 0, 0, map_width, map_height, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void TrailMap::create_compute_program()
{
	GLuint compute = compile_shader("shaders/trail_map.comp", GL_COMPUTE_SHADER);
	compute_program = link_shader_program(1, compute);
}

void TrailMap::create_shader_program()
{
	GLuint vertex   = compile_shader("shaders/trail_map.vert", GL_VERTEX_SHADER);
	GLuint fragment = compile_shader("shaders/trail_map.frag", GL_FRAGMENT_SHADER);

	shader_program = link_shader_program(2, vertex, fragment);
}

void TrailMap::create_quad_vertex_array()
{
	constexpr GLfloat quad_vertices[] = {
		-1.0, -1.0, 0.0, 0.0, 0.0,
		-1.0,  1.0, 0.0, 0.0, 1.0,
		 1.0, -1.0, 0.0, 1.0, 0.0,

		 1.0, -1.0, 0.0, 1.0, 0.0,
		 1.0,  1.0, 0.0, 1.0, 1.0,
		-1.0,  1.0, 0.0, 0.0, 1.0
	};

	GLuint vertex_buffer;
	glCreateBuffers(1, &vertex_buffer);
	glNamedBufferStorage(vertex_buffer, sizeof(quad_vertices), quad_vertices, 0);

	glCreateVertexArrays(1, &quad_vertex_array);
	glVertexArrayVertexBuffer(quad_vertex_array, 0, vertex_buffer, 0, 5 * sizeof(GLfloat));

	glEnableVertexArrayAttrib(quad_vertex_array, 0);
	glEnableVertexArrayAttrib(quad_vertex_array, 1);

	glVertexArrayAttribFormat(quad_vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat));
	glVertexArrayAttribFormat(quad_vertex_array, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

	glVertexArrayAttribBinding(quad_vertex_array, 0, 0);
	glVertexArrayAttribBinding(quad_vertex_array, 1, 0);
}

void TrailMap::draw()
{
	glUseProgram(compute_program);
	send_uniform_float(compute_program, "decay_rate", decay_rate);
	glDispatchCompute(map_width, map_height, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glUseProgram(shader_program);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(quad_vertex_array);
	glBindTextureUnit(0, image);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TrailMap::cleanup()
{
	destroy_shader_program(compute_program);
	destroy_shader_program(shader_program);
	glDeleteTextures(1, &image);
	glDeleteVertexArrays(1, &quad_vertex_array);
}

void Agents::create()
{
    create_storage_buffer();
    create_compute_program();
}

void Agents::create_storage_buffer()
{
    AgentData agents_data;
    agents_data.positions.resize(agents_count);
    agents_data.angles.resize(agents_count);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution_width(0, map_width);
    std::uniform_int_distribution<> distribution_height(0, map_height);
    std::uniform_real_distribution<> distribution_angle(0, 2 * M_PI);

    for(uint32_t i = 0; i < agents_count; ++i)
    {
        agents_data.positions[i].x = distribution_width(generator);
        agents_data.positions[i].y = distribution_height(generator);

        agents_data.angles[i] = distribution_angle(generator);
    }

    glCreateBuffers(1, &agents_data_storage_buffer);
    glNamedBufferStorage(agents_data_storage_buffer, agents_count * (sizeof(vec2) + sizeof(float)), nullptr,  GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(agents_data_storage_buffer, 0, agents_count * sizeof(vec2), agents_data.positions.data());
    glNamedBufferSubData(agents_data_storage_buffer, agents_count * sizeof(vec2), agents_count * sizeof(float), agents_data.angles.data());
}

void Agents::create_compute_program()
{
    GLuint compute = compile_shader("shaders/agents.comp", GL_COMPUTE_SHADER);
    compute_shader_program = link_shader_program(1, compute);
}

void Agents::dispatch()
{
    glUseProgram(compute_shader_program);
    send_uniform_vec2(compute_shader_program, "map_size", map_width, map_height);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, agents_data_storage_buffer);
    glDispatchCompute(agents_count, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
}

void Agents::cleanup()
{
    destroy_shader_program(compute_shader_program);
    glDeleteBuffers(1, &agents_data_storage_buffer);
}
