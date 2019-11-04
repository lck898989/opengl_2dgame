#pragma once
#ifndef BALLOBJECT_H
#define BALLOBJECT_H
#include "gameobject.h"
class BallObject: public GameObject {
public:
	// 半径
	float radius;
	// 是否可以移动
	bool stuck;
	BallObject();
	BallObject(glm::vec2 pos,float radius,glm::vec2 velocity,Texture2D sprite);
	glm::vec2 move(float dt, uint window_width);
	void reset(glm::vec2 position,glm::vec2 velocity);
};
#endif
