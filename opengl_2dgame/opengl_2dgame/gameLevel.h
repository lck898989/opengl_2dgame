#pragma once
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include "gameobject.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "resourcemanager.h"
using namespace std;
class GameLevel {
public:
	// ש��vector����
	vector<GameObject> bricks;
	GameLevel();
	// ���ļ��м��عؿ�
	void load(const char* file, uint levelWidth, uint levelHeight);
	// ��Ⱦ�ؿ�
	void draw(SpriteRenderer &renderer);
	// ���һ���ؿ��Ƿ���ɣ����зǼ�Ӳ�Ĵ�ש�����ݻ٣�
	bool isComplete();
private:
	// ��ש�����ݳ�ʼ���ؿ�
	void init(vector<vector<uint>> tileData,uint levelWidth,uint levelHeight);
};
#endif // !GAMELEVEL_H
