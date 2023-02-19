#include "Application.h"

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

void Application::set_render_mode(const GLenum &mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void Application::resize_window(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::setup()
{
	// Define the vertex and fragment shaders as source code strings
	const char* vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position, 1.0);\n"
		"}\0";

	const char* fragment_shader_source =
		"#version 330 core\n"
		"out vec4 out_color;\n"
		"void main()\n"
		"{\n"
		"	out_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n\0";

	// Compile the vertex and fragment shaders
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);
	check_shader_compilation_success(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);
	check_shader_compilation_success(fragment_shader);

	// Link the shaders into a shader program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	check_program_link_success(shader_program);

	// We don't need the shader objects anymore now that they've been linked, so
	// we can delete them
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	// Define the vertex data for the triangles
	vertices = {
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left
	};

	// Create the vertex buffer object (VBO)
	glGenBuffers(1, &vbo);

	// Bind the VBO and set the vertex data as the buffer's data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(vertices), 
		vertices.data(), 
		GL_STATIC_DRAW
	);

	// Define the face indices for the triangles
	indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Create the element array object (EBO)
	glGenBuffers(1, &ebo);

	// Bind the EBO and set the face index data as the buffer's data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(indices), 
		indices.data(), 
		GL_STATIC_DRAW
	);

	// Create the vertex array object (VAO)
	glGenVertexArrays(1, &vao);

	// Bind the VAO and set the vertex attribute pointer for position
	glBindVertexArray(vao);
	const int pos_attrib = glGetAttribLocation(shader_program, "position");
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(
		pos_attrib, 
		NUM_VERTICES_PER_TRIANGLE, 
		GL_FLOAT,
		GL_FALSE, 
		NUM_VERTICES_PER_TRIANGLE * sizeof(float), 
		nullptr
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Uncomment the below line to enable wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::check_shader_compilation_success(uint32_t shader)
{
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
	}
}

void Application::check_program_link_success(uint32_t program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int log_length = 0;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> log((size_t)log_length);
		glGetShaderInfoLog(program, log_length, &log_length, log.data());
		const std::string log_str(log.begin(), log.end());
		std::cerr << "Shader program linking failed: " << log_str << "\n";
	}
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
					resize_window(event.window.data1, event.window.data2);
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
					set_render_mode(GL_LINE);
					break;
				}
				if (event.key.keysym.sym == SDLK_2)
				{
					set_render_mode(GL_FILL);
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
	// Clear the color buffer to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Specify the shader program to use
	glUseProgram(shader_program);
	// Get the vertex array to render
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Draw the triangles from the vertices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	// Clear the vertex array
	glBindVertexArray(0);
	// Update the framebuffer
	SDL_GL_SwapWindow(window);
}

void Application::initialize()
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create a window
	window = SDL_CreateWindow(
		"OpenGL Example", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		window_width, window_height,
		SDL_WINDOW_OPENGL | 
		SDL_WINDOW_BORDERLESS | 
		SDL_WINDOW_RESIZABLE
	);

	// Create an OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	context = SDL_GL_CreateContext(window);

	// Initialize GLEW to access the OpenGL functions
	glewExperimental = GL_TRUE;
	const GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW.\n";
		return;
	}

	// All libraries are successfully initialized and the application is running
	running = true;
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
	// Clean up resources
	glDeleteProgram(shader_program);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	// Close OpenGL, the SDL window and SDL
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
