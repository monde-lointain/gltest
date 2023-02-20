#version 330 core
out vec4 out_color;
uniform vec3 vcolor;
void main()
{
	out_color = vec4(vcolor, 1.0f);
}
