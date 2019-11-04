#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "spriteRenderer.h"
#include "resourcemanager.h"
#include "gameLevel.h"
#include <stdlib.h>
#include <algorithm>
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};
// 球在砖块中的回弹方向枚举
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
using namespace std;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef tuple<bool, Direction, glm::vec2> CollisionInfo;

class Game {
public:
	GameState state;
	bool keys[1024];
	uint width, height;
	SpriteRenderer* sRenderer;
	// 总的关卡
	vector<GameLevel> levels;
	// 当前处于的关卡
	uint level;
	// 析构函数、构造函数
	Game(uint width, uint height);
	Game();
	~Game();
	// 初始化游戏状态
	void Init();
	// 游戏循环
	void processInput(float dt);
	void update(float dt);
	void render();
	// 做碰撞检测计算
	void doCheckCollistion();
	void resetLevel();
	void resetPlayer();
};

#endif
