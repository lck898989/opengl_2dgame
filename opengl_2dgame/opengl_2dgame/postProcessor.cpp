#include "postProcessor.h"
#include <iostream>
// ������Ч����
PostProcessor::PostProcessor(Shader shader, uint width, uint height) {
	this->texture = Texture2D();
	this->pShader = shader;
	this->width = width;
	this->height = height;
	this->chaos = false;
	this->confuse = false;
	this->shake = false;
	// ��ʼ����Ⱦ�����֡����
	glGenFramebuffers(1,&this->MSFBO);
	glGenFramebuffers(1,&this->FBO);
	// �漰��֡�����е�д��׶� ��ֻд�Ĳ�֧�ֶ�����
	glGenRenderbuffers(1,&this->RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER,this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,8,GL_RGB,width,height);
	// ��������Ȼ�����ص���Ⱦ����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "failed to init MSFBO" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,this->FBO);
	this->texture.generate(this->width, this->height, NULL);
	// ���ӵ�֡������--����������
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "failed to init FBO" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	// ��ʼ����Ⱦ���ݺ�uniformֵ
	this->initRenderData();
	// ʹ����ɫ�������������ɫ���Ļ�����Ͳ���������
	this->pShader.use();
	this->pShader.setInt("tex", 0);
	// ����shake��Ч�������
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // ����
		{  0.0f,    offset  },  // ����
		{  offset,  offset  },  // ����
		{ -offset,  0.0f    },  // ����
		{  0.0f,    0.0f    },  // ����
		{  offset,  0.0f    },  // ����
		{ -offset, -offset  },  // ����
		{  0.0f,   -offset  },  // ����
		{  offset, -offset  }   // ����
	};
	glUniform2fv(glGetUniformLocation(this->pShader.ID, "offsets"), 9, (float*)offsets);
	// ���ñ�ԵЧ��
	int edge_kernel[9] = {
		-1,-1,-1,
		-1,8,-1,
		-1,-1,-1
	};
	glUniform1iv(glGetUniformLocation(this->pShader.ID, "edge_kernel"), 9,edge_kernel);
	// ����ģ������
	float blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->pShader.ID, "blur_kernel"), 9, blur_kernel);
}
// ��ʼ��Ⱦ
void PostProcessor::beginRender() {
	glBindFramebuffer(GL_FRAMEBUFFER,this->MSFBO);
	glClearColor(0.3f,0.4f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}
// ��Ⱦ�����Ļ���Ȼ����Ƶ�����������ȥ
void PostProcessor::endRender() {
	// ��ȡ֡����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	// ����֡����
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	// ��ֻ��֡�����п���������Ϣ������֡������
	glBlitFramebuffer(0, 0, this->width, this->height,0,0,this->width,this->height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
// ��Ⱦ
void PostProcessor::render(float time) {
	this->pShader.use();
	this->pShader.setFloat("time",time);
	this->pShader.setBool("confuse", this->confuse);
	this->pShader.setBool("shake", this->shake);
	this->pShader.setBool("chaos", this->chaos);

	glActiveTexture(GL_TEXTURE0);
	this->texture.bind();
	glBindVertexArray(this->VAO);
	// ������Ļ�ı���
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
