#ifndef __MOLD_H
#define __MOLD_H

#include <cstdint>
#include <vector>

#include "glad/gl.h"

struct vec2 {
    float x, y;
};

struct vec3 {
    float data[3];
};

class TrailMap {
public:
	void create();
	void draw();
	void cleanup();

	uint32_t map_width  {};
	uint32_t map_height {};

	float decay_rate {0.01};

private:
	GLuint image;

	GLuint compute_program;
	GLuint shader_program;

	GLuint quad_vertex_array;

	void create_texture();
	void create_compute_program();
	void create_shader_program();
	void create_quad_vertex_array();
};

class Agents {
public:
    void position_agents_circle();
    void position_agents_uniform();

    void create();
    void dispatch();
    void cleanup();

    uint32_t agents_count {};

    uint32_t map_width  {};
    uint32_t map_height {};

private:
    struct AgentData
    {
        std::vector<vec2> positions;
        std::vector<float> angles;
    };

    GLuint agents_data_storage_buffer;

    GLuint compute_shader_program;

    void create_storage_buffer();
    void create_compute_program();
};

#endif
