#pragma once
#ifndef MYFONT_H
#define MYFONT_H
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.h"
#include <map>
using namespace std;
typedef unsigned int uint;
// 每一个字体所需要的参数
struct Character
{
	// 字形纹理的id
	uint textureId;
	// 字形的大小
	glm::ivec2 size;
	// 从基准线到字形的左边界，上部边界的距离
	glm::ivec2 bearing;
	// 原点距离下一个字形原点的距离
	uint advance;
};
class MyFont {
public:
	FT_Library ft;
	FT_Face face;
	uint VAO,VBO;
	Shader fontShader;
	// 一个字符对应一个字符对象形成影射关系
	map<char, Character> characters;
	// 根据char的名字获得字体对象
	Character getCharByName(const char& name);
	// 初始化characters
	MyFont(Shader& shader);
	~MyFont();
	void draw(string text, float x, float y, float scale, glm::vec3 color);
	void initRenderData();
};

#endif // !MYFONT_H
