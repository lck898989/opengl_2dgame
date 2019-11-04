#pragma once
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include "texture.h"
#include "shader.h"
class SpriteRenderer {
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();
	// 在头文件进行声明默认参数，在实现文件中不用声明了
	void drawSprite(Texture2D &texture,glm::vec2 position,glm::vec2 size = glm::vec2(10,10),float rotate = 0.0f,glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	uint quadVAO;
	// 发送顶点数据
	void initRenderData(float* vertices,uint sizeVertices);
};
#endif