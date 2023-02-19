#pragma once

#include <array>
#include <cstdint>

#include <SDL2/SDL.h>

constexpr int NUM_TRIANGLES = 2;
constexpr int NUM_VERTICES = 4;
constexpr int NUM_VERTICES_PER_TRIANGLE = 3;

typedef uint32_t GLenum;

class Application
{
public:
	void initialize();
	void run();
	void setup();
	void input();
	void update();
	void render();
	void destroy();

private:
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	int window_width = 800;
	int window_height = 600;
	float target_seconds_per_frame = 0.0f;
	bool running = false;

	uint32_t num_vertices = 0;
	uint32_t vbo = 0;
	uint32_t ebo = 0;
	uint32_t vao = 0;
	uint32_t vertex_shader = 0;
	uint32_t fragment_shader = 0;
	uint32_t shader_program = 0;

	std::array<float, (size_t)(NUM_VERTICES * NUM_VERTICES_PER_TRIANGLE)> vertices{};
	std::array<uint32_t, (size_t)(NUM_TRIANGLES * NUM_VERTICES_PER_TRIANGLE)> indices{};

	static void resize_window(int width, int height);
	static void check_shader_compilation_success(uint32_t shader);
	static void check_program_link_success(uint32_t program);
	static void set_render_mode(const GLenum &mode);
};

