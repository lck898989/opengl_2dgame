#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <iostream>
#include <string>
#include "stb_image.h"
#include <GLFW/glfw3.h>
using namespace std;
typedef unsigned int uint;
typedef unsigned char uchar;
class Texture2D {
public:
	uint ID;
	uint width, height;
	uint format_internal;
	uint format_image;
	uint wrap_s;
	uint wrap_t;
	uint filter_Min;
	uint filter_Max;
	Texture2D();
	void generate(uint width, uint height, uchar *data);
	// 绑定一个贴图到一个当前活跃状态
	void bind();
};
#endif // !TEXTURE_H
