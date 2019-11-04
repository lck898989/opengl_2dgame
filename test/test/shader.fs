#version 330 core
in vec3 outputColor;
out vec4 fragColor;
uniform vec4 outcolor;
void main(){
	fragColor = vec4(outputColor,1.0f);
}