#include "ballobject.h"

BallObject::BallObject() :GameObject(){
	this->radius = 12.5f;
	this->stuck = true;

}
BallObject::BallObject(glm::vec2 pos,float radius,glm::vec2 velocity,Texture2D sprite): GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity) {
	this->radius = radius;
	this->stuck = true;
}
glm::vec2 BallObject::move(float dt,uint window_width) {
	// 如果不移动挡板的话主角可以移动
	if (!this->stuck) {
		this->position += this->velocity * dt;
		if (this->position.x <= 0.0f) {
			this->position.x = 0.0f;
			this->velocity.x = -this->velocity.x;
		}
		else if (this->position.x >= window_width - this->size.x) {
			this->position.x = window_width - this->size.x;
			this->velocity.x = -this->velocity.x;
		}
		if (this->position.y <= 0.0f) {
			this->velocity.y = -this->velocity.y;
			this->position.y = 0.0f;
		}
	}
	return this->position;
}
void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
}
