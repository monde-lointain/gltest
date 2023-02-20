#include "Renderer.h"

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include "../Shader/Shader.h"

void Renderer::set_render_mode(const GLenum &mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void Renderer::resize_window(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::create_shaders()
{
	// Create the shader from the source code
	shader = Shader("./shaders/2dvertex.glsl", "./shaders/2dfragment.glsl");

	// Define the vertex data for the triangles
	vertices[0] = {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}; // bottom left
	vertices[1] = {glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}; // bottom right
	vertices[2] = {glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)}; // top

	// Create the vertex buffer object (VBO)
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(vertices), 
		vertices.data(), 
		GL_STATIC_DRAW
	);

	// Define the face indices for the triangles
	indices = {0, 1, 2};

	// Create the element array object (EBO)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(indices), 
		indices.data(), 
		GL_STATIC_DRAW
	);

	// Create the vertex array object (VAO)
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Set up vertex attribute pointers for position and color
	const int pos_attr = glGetAttribLocation(shader.program, "position");
	const void* pos_attr_offset = reinterpret_cast<void*>(0);
	glEnableVertexAttribArray(pos_attr);
	glVertexAttribPointer(
		pos_attr, 
		NUM_VERTICES_PER_TRIANGLE, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		pos_attr_offset
	);

	const int col_attr = glGetAttribLocation(shader.program, "color");
	const void* col_attr_offset = reinterpret_cast<void*>(sizeof(float) * 3);
	glEnableVertexAttribArray(col_attr);
	glVertexAttribPointer(
		col_attr,
		NUM_VERTICES_PER_TRIANGLE,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		col_attr_offset
	);
}

bool Renderer::initialize()
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
		return false;
	}

	return true;
}

void Renderer::render()
{
	// Clear the color buffer to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Specify the shader program to use
	glUseProgram(shader.program);

	// Update the uniform color
	const float t = (float)SDL_GetTicks() / 1000.0f;
	const float green = (sinf(t) / 2.0f) + 0.5f;
	const glm::vec3 color(0.0f, green, 0.0f);
	shader.set_uniform("vcolor", color);

	const glm::vec3 offset(0.5f, 0.0f, 0.0f);
	shader.set_uniform("offset", offset);

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

void Renderer::destroy()
{
	// Clean up resources
	shader.destroy();
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	// Close OpenGL, the SDL window and SDL
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
