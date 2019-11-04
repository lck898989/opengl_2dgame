#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include "texture.h"
#include "spriteRenderer.h"
/*
  如果virtual void draw() = 0;是一个纯虚函数不能实例化该对象
  抽象类，不能被实例化

*/
class GameObject {
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	// 是否坚固
	bool isSolid;
	// 是否被销毁
	bool isDestroy;
	Texture2D sprite;
	GameObject();
	GameObject(glm::vec2 pos,glm::vec2 size,Texture2D sprite,glm::vec3 color = glm::vec3(1.0f),glm::vec2 velocity = glm::vec2(0.0f,0.0f));
	// 每个对象的绘制方法不同用到了虚函数（类似java中接口的定义）该类为抽象类，谁继承了该类就必须实现该方法
	virtual void draw(SpriteRenderer &renderer);
};
#endif // !GAMEOBJECT_H

