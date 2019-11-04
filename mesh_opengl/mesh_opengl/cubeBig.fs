#version 330 core
in vec2 textureCoord;
out vec4 fragColor;
uniform sampler2D tex;
void main() {
	fragColor = vec4(0.04, 0.28, 0.26, 1.0);
}