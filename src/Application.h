#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <cstdint>

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

    uint32_t vbo = 0;
    uint32_t vao = 0;
    uint32_t vertex_shader = 0;
    uint32_t fragment_shader = 0;
    uint32_t shader_program = 0;

    std::array<float, 9> vertices{};

    static void resize_window(int width, int height);
    static void check_shader_compilation_success(uint32_t shader);
    static void check_program_link_success(uint32_t program);
};

