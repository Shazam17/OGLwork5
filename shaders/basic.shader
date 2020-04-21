#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 u_proj;

void main() {
	gl_Position = u_proj*vec4(pos, -2, 1);
}


#shader fragment
#version 330 core

out vec4 color;

void main() {
	color = vec4(1, 1, 0, 1);
}

