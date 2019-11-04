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
	// 砖块vector容器
	vector<GameObject> bricks;
	GameLevel();
	// 从文件中加载关卡
	void load(const char* file, uint levelWidth, uint levelHeight);
	// 渲染关卡
	void draw(SpriteRenderer &renderer);
	// 检查一个关卡是否完成（所有非坚硬的瓷砖均被摧毁）
	bool isComplete();
private:
	// 由砖块数据初始化关卡
	void init(vector<vector<uint>> tileData,uint levelWidth,uint levelHeight);
};
#endif // !GAMELEVEL_H
