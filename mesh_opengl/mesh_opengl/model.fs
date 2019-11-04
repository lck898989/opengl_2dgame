#version 330 core
in vec3 normal;
in vec2 textureCoord;
out vec4 fragColor;
uniform sampler2D texture_diffuse1;
void main() {
	fragColor = texture(texture_diffuse1,textureCoord);
}