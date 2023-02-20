#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertex_shader_file,
	const std::string& fragment_shader_file)
{
	// Load and compile the shader source code
	const uint32_t vertex_shader = load_shader(vertex_shader_file, GL_VERTEX_SHADER);
	const uint32_t fragment_shader = load_shader(fragment_shader_file, GL_FRAGMENT_SHADER);

	// Create the shader program
	program = glCreateProgram();

	// Attach the compiled shaders
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	// Check for linker errors
	int success; 
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> log((size_t)log_length);
		glGetProgramInfoLog(program, log_length, &log_length, log.data());
		const std::string log_str(log.begin(), log.end());
		std::cerr << "Shader program linking failed: " << log_str << "\n";

		// Delete the program if linking failed
		glDeleteProgram(program);
		program = 0;
		return;
	}

	// Detatch and delete the shaders now that we're done with them
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

uint32_t Shader::load_shader(const std::string& filename, GLenum shader_type)
{
	// Open the shader file
	const std::ifstream file(filename);
	if (!file)
	{
		std::cerr << "Unable to open shader file: " << filename << "/n";
		return 0;
	}

	// Load the shader source code
	std::stringstream ss;
	ss << file.rdbuf();
	const std::string source = ss.str();

	// Create and compile the shader
	const uint32_t shader = glCreateShader(shader_type);
	const char* source_ptr = source.c_str();
	const int source_length = (int)source.length();
	glShaderSource(shader, 1, &source_ptr, &source_length);
	glCompileShader(shader);

	// Check for compilation errors
	int success; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> log((size_t)log_length);
		glGetShaderInfoLog(shader, log_length, &log_length, log.data());
		const std::string log_str(log.begin(), log.end());
		std::cerr << "Shader compilation failed: " << log_str << "\n";

		// Delete the shader if compilation failed
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void Shader::set_uniform(const char* name, float value) const
{
	const int location = glGetUniformLocation(program, name);
	if (location != -1)
	{
		glUniform1f(location, value);
	}
}

void Shader::set_uniform(const char* name, const glm::vec3& value) const
{
	const int location = glGetUniformLocation(program, name);
	if (location != -1)
	{
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
}

void Shader::destroy() const
{
	glDeleteProgram(program);
}

Shader create_shader(const std::string& vertex_shader_file,
	const std::string& fragment_shader_file)
{
	Shader shader = Shader(vertex_shader_file, fragment_shader_file);
	return shader;
}
