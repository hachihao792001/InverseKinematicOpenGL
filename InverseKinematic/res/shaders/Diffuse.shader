#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;
layout(location = 3) in vec3 normal;
layout(location = 4) in float useColor;
layout(location = 5) in vec4 color;

uniform mat4 u_CamMatrix;
uniform mat4 u_ModelMatrix;

out float v_TexIndex;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec4 v_CurrentPos;
out float v_UseColor;
out vec4 v_Color;

void main() {
	v_CurrentPos = u_ModelMatrix * position;
	gl_Position = u_CamMatrix * v_CurrentPos;

	v_TexIndex = texIndex;
	v_TexCoord = texCoord;
	v_Normal = normal;
	v_UseColor = useColor;
	v_Color = color;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in float v_TexIndex;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec4 v_CurrentPos;
in float v_UseColor;
in vec4 v_Color;

uniform sampler2D u_Textures[7];
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform float u_Intensity;

vec4 pointLight() {
	float ambient = 0.2f;

	// diffuse lighting
	vec3 normal = normalize(v_Normal);
	vec4 lightDirection = normalize(u_LightPos - v_CurrentPos);
	float diffuse = max(dot(normal, vec3(lightDirection)) * u_Intensity, 0.0f);

	vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
	return (texColor * u_LightColor * (diffuse + ambient) + vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

vec4 dirLight() {
	float ambient = 0.5f;

	// diffuse lighting
	vec3 normal = normalize(v_Normal);
	vec4 lightDirection = normalize(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	float diffuse = max(dot(normal, vec3(lightDirection)) * u_Intensity, 0.0f);

	vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
	return (texColor * u_LightColor * (diffuse + ambient) + vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void main() {
	if (v_UseColor == 1.0f)
		color = v_Color;
	else
		color = pointLight();
};