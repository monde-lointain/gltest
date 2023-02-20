#include "Application.h"

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

Application::Application()
{
	renderer = std::make_shared<Renderer>();
}

void Application::setup()
{
	renderer->create_shaders();
}

void Application::input()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		// Handle all SDL events
		switch (event.type)
		{
			// Closing the window
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			// Resizing the window
			case SDL_WINDOWEVENT:
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					renderer->resize_window(event.window.data1, event.window.data2);
					break;
				}
				break;
			}
			case SDL_KEYDOWN:
			{
				// Close the window
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
					break;
				}
				// Switch render modes
				if (event.key.keysym.sym == SDLK_1)
				{
					Renderer::set_render_mode(GL_LINE);
					break;
				}
				if (event.key.keysym.sym == SDLK_2)
				{
					Renderer::set_render_mode(GL_FILL);
					break;
				}
				break;
			}
		}
	}
}

void Application::update()
{
}

void Application::render()
{
	renderer->render();
}

void Application::initialize()
{
	// All libraries are successfully initialized and the application is running
	running = renderer->initialize();
}

void Application::run()
{
	setup();

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	// We want the FPS to be fixed to the speed of the user's monitor refresh
	target_seconds_per_frame = 1.0f / (float)display_mode.refresh_rate;

	uint64_t start, end;
	float time_elapsed;
	uint32_t ms_to_wait;

	while (running)
	{
		start = SDL_GetPerformanceCounter();
		input();
		update();
		render();
		end = SDL_GetPerformanceCounter();

		time_elapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();

		// If we still have time after updating the frame, wait to advance to
		// the next one
		if (time_elapsed < target_seconds_per_frame)
		{
			ms_to_wait = (uint32_t)((target_seconds_per_frame - time_elapsed) * 1000.0f);
			SDL_Delay(ms_to_wait);
		}
	}
}

void Application::destroy()
{
	renderer->destroy();
}
