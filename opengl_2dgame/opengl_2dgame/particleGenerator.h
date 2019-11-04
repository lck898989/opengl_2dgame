#pragma once
/*

	粒子生成类

*/
#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"
#include "gameobject.h"
struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	Particle() :position(0.0f), velocity(0.0f), color(0.0f), life(0.0f) {};
};
class ParticleGenerator {
public:
	ParticleGenerator(Shader shader,Texture2D texture,uint amount);
	// update all particles;
	void update(float dt, GameObject& object, uint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// render all particles;
	void draw();
private:
	vector<Particle> particles;
	uint amount;
	Shader shader;
	Texture2D texture;
	uint VAO;
	void init();
	uint firstUnusedParticle();
	void respawnParticle(Particle& partilcle,GameObject& object,glm::vec2 offset = glm::vec2(0.0f));
};
#endif // !PARTICLEGENERATOR_H
