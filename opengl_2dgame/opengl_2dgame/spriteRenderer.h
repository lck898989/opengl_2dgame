#pragma once
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include "texture.h"
#include "shader.h"
class SpriteRenderer {
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();
	// ��ͷ�ļ���������Ĭ�ϲ�������ʵ���ļ��в���������
	void drawSprite(Texture2D &texture,glm::vec2 position,glm::vec2 size = glm::vec2(10,10),float rotate = 0.0f,glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	uint quadVAO;
	// ���Ͷ�������
	void initRenderData(float* vertices,uint sizeVertices);
};
#endif