#version 330 core
out vec4 fragColor;
in vec2 texCoord;
uniform vec3 spriteColor;
uniform sampler2D tex;

void main() {
	fragColor = vec4(spriteColor,1.0f) * texture(tex,texCoord);
}