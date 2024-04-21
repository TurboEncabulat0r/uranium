#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 fColor;
out vec2 fTexCoord;
out vec3 fPosition;

void main() {
	fColor = color;
	fTexCoord = texCoord;
	fPosition = vec3(position.x, 0, position.z);
	gl_Position = vec4(vec3(position.x, 0, position.z), 1.0);
}