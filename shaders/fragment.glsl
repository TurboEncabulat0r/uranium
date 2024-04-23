#version 440

in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler; // Texture sampler uniform

void main() {
    // Sample from the texture using texture coordinates
    vec4 texColor = texture(textureSampler, fTexCoord);
    
    // Blend the sampled texture color with the vertex color
    FragColor = vec4(texColor.rgb * fColor, texColor.a);
}