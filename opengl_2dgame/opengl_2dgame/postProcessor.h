#pragma once
#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H
#include "shader.h"
#include "shader.h"
#include "spriteRenderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class PostProcessor {
public:
	Shader pShader;
	Texture2D texture;
	uint width, height;
	bool confuse, shake, chaos;
	PostProcessor(Shader shader, uint width, uint height);
	void beginRender();
	void endRender();
	void render(float time);
private:
	uint MSFBO, FBO;
	uint RBO;
	uint VAO;
	void initRenderData();
};
#endif // !POSTPROCESSOR_H
