#pragma once

#include <cstdint>
#include <string>
#include <glm/vec3.hpp>

typedef uint32_t GLenum;

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vertex_shader_file,
		const std::string& fragment_shader_file);

	void destroy() const;

	uint32_t program = 0;

private:
	static uint32_t load_shader(const std::string& filename, GLenum shader_type);

public:
	void set_uniform(const char* name, float value) const;
	void set_uniform(const char* name, const glm::vec3& value) const;
};

Shader create_shader(const std::string& vertex_shader_file,
	const std::string& fragment_shader_file);
