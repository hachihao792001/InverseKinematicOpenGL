#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

uniform mat4 u_MVP;

out float v_TexIndex;
out vec2 v_TexCoord;

void main() {
	gl_Position = u_MVP * position;

	v_TexIndex = texIndex;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in float v_TexIndex;
in vec2 v_TexCoord;

uniform sampler2D u_Textures[2];

void main() {
	int index = int(v_TexIndex);
	vec4 texColor = texture(u_Textures[index], v_TexCoord);
	color = texColor;
};