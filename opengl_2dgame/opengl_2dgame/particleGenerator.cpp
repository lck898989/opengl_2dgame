#include "particleGenerator.h"
// ���һ��û�б�ʹ�õ�����
uint lastUsedParticle = 0;


ParticleGenerator::ParticleGenerator(Shader shader,Texture2D texture,uint amount):shader(shader),texture(texture),amount(amount) {
	this->init();
}
// ��������״̬
void ParticleGenerator::update(float dt,GameObject& object,uint newParticles,glm::vec2 offset) {
	// ��������µ�����
	for (uint i = 0; i < newParticles; ++i) {
		uint unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	for (uint i = 0; i < this->amount; ++i) {
		Particle& p = this->particles[i];
		p.life -= dt;
		if (p.life > 0.0f) {
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5;
		}
	}
}
// ��ʼ������
void ParticleGenerator::init() {
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// ��һ��������װ������������ȥ
	for (uint i = 0; i < this->amount; i++) {
		this->particles.push_back(Particle());
	}
}
// �ҵ���һ�����ٵ����Ӷ��������λ��
uint ParticleGenerator::firstUnusedParticle() {
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}
// 
void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset) {
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5 + ((rand() % 100) / 100.0f);
	if (rColor >= 1.0f) {
		rColor = 1.0f;
	}
	particle.position = object.position + random + offset;
	cout << "rColor is " << rColor << endl;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 0.3f;
	particle.velocity = object.velocity * 0.1f;
}
// �������Ӷ���
void ParticleGenerator::draw() {
	// ���û��
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	this->shader.use();
	for (Particle particle : this->particles) {
		if (particle.life > 0.0f) {
			this->shader.setVec2("offset", particle.position);
			particle.color.a = 0.5f;
			this->shader.setVec4("color", particle.color);
			this->texture.bind();

			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES,0,6);
			glBindVertexArray(0);
		}
	}
	// �ָ���Ĭ��
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
