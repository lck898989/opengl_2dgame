#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 outputColor;
float tempx;
float tempy;
uniform float offsetY;
uniform float degree;
void main()
{
    // …Ë÷√Œª÷√
    tempx = aPos.x*cos(radians(degree)) - aPos.y * sin(radians(degree));
	tempy = aPos.x*sin(radians(degree)) + aPos.y * cos(radians(degree));
	gl_Position = vec4(tempx,tempy - offsetY,aPos.z,1.0f);
    outputColor = aColor;
}