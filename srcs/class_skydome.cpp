#include "class_skydome.h"
#include "pfm/pfm.hpp"
#include <cmath>

Skydome::Skydome()
{}

Skydome::~Skydome()
{}

void Skydome::ComputePositions(float const& radius, size_t const& num_rows, size_t const& num_cols)
{
	_radius = radius;
	int top_strip_num_vertices = 6 * num_cols;
	int regular_strip_num_vertices = 6 * num_cols;
	//minus 1 because of top strip
	_num_vertices = top_strip_num_vertices + ((num_rows - 1) * regular_strip_num_vertices);

	_vertices = std::vector<Vertex>(_num_vertices);

	float heading_increment = 360.f / static_cast<float>(num_cols);
	float pitch_increment = 90.f / static_cast<float>(num_rows);
	float u_increment = heading_increment / 360.f;
	float v_increment = pitch_increment / 180.f;

	size_t i = 0;
	
	// Top strips
	for (float heading = 0.f, u = 0.f ; heading < 360.f ; heading += heading_increment, u += u_increment) {
		pfm::vec3 v0(0.f, radius, 0.f); 
		pfm::vec2 v0_tex_coords(u - (u/2.f), 1.f);
		pfm::vec3 v1(0.f, radius, 0.f); 
		pfm::vec2 v1_tex_coords(u, 1.f);

		pfm::vec3 v2 = pfm::sphericalToCartesian(radius, -pitch_increment, heading);
		pfm::vec2 v2_tex_coords(u, 1.f - v_increment);
		pfm::vec3 v3 = pfm::sphericalToCartesian(radius, -pitch_increment, heading+heading_increment);
		pfm::vec2 v3_tex_coords(u + u_increment, 1.f - v_increment);

		_vertices[i].tex_coords = v0_tex_coords;
		_vertices[i++].position = v0;
		_vertices[i].tex_coords = v1_tex_coords;
		_vertices[i++].position = v1;
		_vertices[i].tex_coords = v2_tex_coords;
		_vertices[i++].position = v2;

		_vertices[i].tex_coords = v1_tex_coords;
		_vertices[i++].position = v1;
		_vertices[i].tex_coords = v3_tex_coords;
		_vertices[i++].position = v3;
		_vertices[i].tex_coords = v2_tex_coords;
		_vertices[i++].position = v2;
	}

 	// Regular strips
	for (float pitch = -90.f, v = 0.f; pitch < -pitch_increment ; pitch += pitch_increment, v += v_increment) {
		for (float heading = 0.f, u = 0.f ; heading < 360.f ; heading += heading_increment, u += u_increment) {
			pfm::vec3 v0 = pfm::sphericalToCartesian(radius, pitch, heading);
			pfm::vec2 v0_tex_coords(u, v);
			pfm::vec3 v1 = pfm::sphericalToCartesian(radius, pitch, heading+heading_increment);
			pfm::vec2 v1_tex_coords(u + u_increment, v);
			pfm::vec3 v2 = pfm::sphericalToCartesian(radius, pitch+pitch_increment, heading);
			pfm::vec2 v2_tex_coords(u, v + v_increment);
			pfm::vec3 v3 = pfm::sphericalToCartesian(radius, pitch+pitch_increment, heading+heading_increment);
			pfm::vec2 v3_tex_coords(u + u_increment, v + v_increment);

			assert(i + 6 <= _num_vertices);

			_vertices[i].tex_coords = v0_tex_coords;
			_vertices[i++].position = v0;
			_vertices[i].tex_coords = v1_tex_coords;
			_vertices[i++].position = v1;
			_vertices[i].tex_coords = v2_tex_coords;
			_vertices[i++].position = v2;

			_vertices[i].tex_coords = v1_tex_coords;
			_vertices[i++].position = v1;
			_vertices[i].tex_coords = v3_tex_coords;
			_vertices[i++].position = v3;
			_vertices[i].tex_coords = v2_tex_coords;
			_vertices[i++].position = v2;
		}
	}

	for (size_t i = 0 ; i < _vertices.size() ; i++) {
		_vertices[i].color = {
			static_cast<float>(rand()%255) / 255.f,
			static_cast<float>(rand()%255) / 255.f,
			static_cast<float>(rand()%255) / 255.f
		};
	}
}

void Skydome::ComputeTexCoords()
{
	float max_u = 0.f;
	float min_u = 0.f;
	float max_v = 0.f;
	float min_v = 0.f;

	for (size_t i = 0 ; i < _num_vertices ; i++) {
		pfm::vec3 v = _vertices[i].position;
		float kR = _radius;

		_vertices[i].tex_coords.u = kR * tan(v.x / kR);
		_vertices[i].tex_coords.v = kR * tan(v.z / kR);

		max_u = std::max(max_u, _vertices[i].tex_coords.u);
		max_v = std::max(max_v, _vertices[i].tex_coords.v);
		min_u = std::min(min_u, _vertices[i].tex_coords.u);
		min_v = std::min(min_v, _vertices[i].tex_coords.v);
	}

	max_u += std::abs(min_u);
	max_v += std::abs(min_v);

	for (size_t i = 0 ; i < _num_vertices ; i++) {
		_vertices[i].tex_coords.u += std::abs(min_u);
		_vertices[i].tex_coords.v += std::abs(min_v);

		_vertices[i].tex_coords.u /= max_u;
		_vertices[i].tex_coords.v /= max_v;
	}
}

void Skydome::SendBuffers()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

	// Link
	/* Position */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	/* Normal */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	/* tex_coords */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coords)));
	/* Color */
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skydome::InitDepthMap()
{
	glGenFramebuffers(1, &depth_map_FBO);

	// Texture
	glGenTextures(1, &depth_map_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth_map_texture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Color
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	// Attach texture to framebuffer as depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depth_map, 0); // Color
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Shader
	depth_map_shader.LoadShaders("./shaders/sm.vert", "./shaders/sm.frag");
}

void Skydome::DrawDepthMap(int frames, Engine & e)
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
	glClearColor(0.f, 0.2f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_DEPTH_TEST);
	pfm::mat4 rotated_sun_mat = pfm::rotate(pfm::mat4(1.f), static_cast<float>(frames) * pfm::radians(0.01), pfm::vec3(0.f, 0.f, 1.f));
	pfm::vec4 sp = rotated_sun_mat * pfm::vec4(e.sun_position.x, e.sun_position.y, e.sun_position.z, 1.f);
	depth_map_shader.SetViewMat(pfm::lookAt(pfm::vec3(sp.x, sp.y, sp.z), e.camera.position, pfm::vec3(1.f, 0.f, 0.f)));
	depth_map_shader.SetModelMat(pfm::mat4(1.f));
	DrawWith(frames, e, depth_map_shader);
}