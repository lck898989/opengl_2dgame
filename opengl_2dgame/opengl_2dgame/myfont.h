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
// ÿһ����������Ҫ�Ĳ���
struct Character
{
	// ���������id
	uint textureId;
	// ���εĴ�С
	glm::ivec2 size;
	// �ӻ�׼�ߵ����ε���߽磬�ϲ��߽�ľ���
	glm::ivec2 bearing;
	// ԭ�������һ������ԭ��ľ���
	uint advance;
};
class MyFont {
public:
	FT_Library ft;
	FT_Face face;
	uint VAO,VBO;
	Shader fontShader;
	// һ���ַ���Ӧһ���ַ������γ�Ӱ���ϵ
	map<char, Character> characters;
	// ����char�����ֻ���������
	Character getCharByName(const char& name);
	// ��ʼ��characters
	MyFont(Shader& shader);
	~MyFont();
	void draw(string text, float x, float y, float scale, glm::vec3 color);
	void initRenderData();
};

#endif // !MYFONT_H
