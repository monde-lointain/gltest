#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <SDL2/SDL.h>

#include "Vertex.h"
#include "../Shader/Shader.h"

constexpr int NUM_TRIANGLES = 1;
constexpr int NUM_VERTICES = 3;
constexpr int NUM_VERTICES_PER_TRIANGLE = 3;

typedef uint32_t GLenum;
class Shader;

class Renderer
{
public:
	bool initialize();
	void create_shaders();
	void render();
	void destroy();

private:
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	int window_width = 800;
	int window_height = 600;

	uint32_t vbo = 0; // vertex buffer object
	uint32_t ebo = 0; // element buffer object
	uint32_t vao = 0; // vertex array object
	Shader shader;

	std::array<Vertex, NUM_VERTICES> vertices{};
	std::array<uint32_t, (size_t)(NUM_TRIANGLES * NUM_VERTICES_PER_TRIANGLE)> indices{};

public:
	static void resize_window(int width, int height);
	static void set_render_mode(const GLenum &mode);
};

