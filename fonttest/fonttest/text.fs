#version 330 core
in vec2 texCoord;
uniform sampler2D tex;
uniform vec3 textColor;

out vec4 color;
void main() {
	color = vec4(textColor,1.0f) * vec4(1.0f,1.0f,1.0f,texture(tex,texCoord).r);
}