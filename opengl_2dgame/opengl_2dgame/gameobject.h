#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include "texture.h"
#include "spriteRenderer.h"
/*
  ���virtual void draw() = 0;��һ�����麯������ʵ�����ö���
  �����࣬���ܱ�ʵ����

*/
class GameObject {
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	// �Ƿ���
	bool isSolid;
	// �Ƿ�����
	bool isDestroy;
	Texture2D sprite;
	GameObject();
	GameObject(glm::vec2 pos,glm::vec2 size,Texture2D sprite,glm::vec3 color = glm::vec3(1.0f),glm::vec2 velocity = glm::vec2(0.0f,0.0f));
	// ÿ������Ļ��Ʒ�����ͬ�õ����麯��������java�нӿڵĶ��壩����Ϊ�����࣬˭�̳��˸���ͱ���ʵ�ָ÷���
	virtual void draw(SpriteRenderer &renderer);
};
#endif // !GAMEOBJECT_H

