#version 330 core
layout (location = 0) in vec3 position;
uniform vec3 offset;
void main()
{
	gl_Position = vec4(position + offset, 1.0);
}
