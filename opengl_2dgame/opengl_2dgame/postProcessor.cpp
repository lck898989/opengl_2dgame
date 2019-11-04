#include "postProcessor.h"
#include <iostream>
// 后期特效处理
PostProcessor::PostProcessor(Shader shader, uint width, uint height) {
	this->texture = Texture2D();
	this->pShader = shader;
	this->width = width;
	this->height = height;
	this->chaos = false;
	this->confuse = false;
	this->shake = false;
	// 初始化渲染缓冲和帧缓冲
	glGenFramebuffers(1,&this->MSFBO);
	glGenFramebuffers(1,&this->FBO);
	// 涉及到帧缓冲中的写入阶段 是只写的不支持读操作
	glGenRenderbuffers(1,&this->RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER,this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,8,GL_RGB,width,height);
	// 关联到深度缓冲相关的渲染缓冲
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "failed to init MSFBO" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,this->FBO);
	this->texture.generate(this->width, this->height, NULL);
	// 附加到帧缓冲上--关联到纹理
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "failed to init FBO" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	// 初始化渲染数据和uniform值
	this->initRenderData();
	// 使用着色器如果不适用着色器的话缓冲就不会起作用
	this->pShader.use();
	this->pShader.setInt("tex", 0);
	// 设置shake特效所需参数
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // 左上
		{  0.0f,    offset  },  // 中上
		{  offset,  offset  },  // 右上
		{ -offset,  0.0f    },  // 左中
		{  0.0f,    0.0f    },  // 正中
		{  offset,  0.0f    },  // 右中
		{ -offset, -offset  },  // 左下
		{  0.0f,   -offset  },  // 中下
		{  offset, -offset  }   // 右下
	};
	glUniform2fv(glGetUniformLocation(this->pShader.ID, "offsets"), 9, (float*)offsets);
	// 设置边缘效果
	int edge_kernel[9] = {
		-1,-1,-1,
		-1,8,-1,
		-1,-1,-1
	};
	glUniform1iv(glGetUniformLocation(this->pShader.ID, "edge_kernel"), 9,edge_kernel);
	// 设置模糊参数
	float blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->pShader.ID, "blur_kernel"), 9, blur_kernel);
}
// 开始渲染
void PostProcessor::beginRender() {
	glBindFramebuffer(GL_FRAMEBUFFER,this->MSFBO);
	glClearColor(0.3f,0.4f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}
// 渲染正常的画面然后绘制到离屏缓冲中去
void PostProcessor::endRender() {
	// 读取帧缓冲
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	// 绘制帧缓冲
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	// 从只读帧缓冲中拷贝像素信息到绘制帧缓冲中
	glBlitFramebuffer(0, 0, this->width, this->height,0,0,this->width,this->height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
// 渲染
void PostProcessor::render(float time) {
	this->pShader.use();
	this->pShader.setFloat("time",time);
	this->pShader.setBool("confuse", this->confuse);
	this->pShader.setBool("shake", this->shake);
	this->pShader.setBool("chaos", this->chaos);

	glActiveTexture(GL_TEXTURE0);
	this->texture.bind();
	glBindVertexArray(this->VAO);
	// 绘制屏幕四边形
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindVertexArray(0);
}
void PostProcessor::initRenderData() {
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
