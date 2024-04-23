#version 440

in vec3 fColor;
in vec2 fTexCoord;
in vec3 fPosition;

out vec4 FragColor;

void main() {
	FragColor = vec4(fColor, 1.0);
}