#include "gameobject.h"
GameObject::GameObject() {
	this->position = glm::vec2(0,0);
	this->size = glm::vec2(0,0);
	this->velocity = glm::vec2(0.0f);
	this->color = glm::vec3(1.0f);
	this->sprite = Texture2D();

	this->isSolid = false;
	this->isDestroy = false;
}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) {
	this->position = pos;
	this->size = size;
	this->sprite = sprite;
	this->color = color;
	this->velocity = velocity;
	// 默认的rotation为0度
	this->rotation = 0;

	this->isSolid = false;
	this->isDestroy = false;
}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(this->sprite,this->position,this->size,this->rotation,this->color);
}