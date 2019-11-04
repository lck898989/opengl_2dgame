#include "spriteRenderer.h"
SpriteRenderer::SpriteRenderer(Shader &shader) {
	this->shader = shader;
	float vertices[] = {
		// 位置     // 纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	// 初始化顶点数据
	this->initRenderData(vertices,sizeof(vertices));
}
SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1,&this->quadVAO);
}
void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
	// 准备变换
	this->shader.use();
	//glEnable(GL_BLEND);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model,glm::vec3(-0.5f * size.x,-0.5f * size.y,0.0f));
	model = glm::scale(model, glm::vec3(size,1.0f));
	this->shader.setMat4("model",model);
	this->shader.setVec3("spriteColor",color);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES,0,6);
	// 绘制完毕之后释放
	glBindVertexArray(0);


}
void SpriteRenderer::initRenderData(float* vertices,uint size) {
	uint VBO;
	if (vertices == nullptr) {
		cout << "顶点数据缺失" << endl;
		return;
	}
	glGenVertexArrays(1,&this->quadVAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(this->quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4 * sizeof(float),(void *)0);
	glEnableVertexAttribArray(0);
	// 解除绑定
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}