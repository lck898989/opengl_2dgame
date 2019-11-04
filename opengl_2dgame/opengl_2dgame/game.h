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
// ����ש���еĻص�����ö��
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
	// �ܵĹؿ�
	vector<GameLevel> levels;
	// ��ǰ���ڵĹؿ�
	uint level;
	// �������������캯��
	Game(uint width, uint height);
	Game();
	~Game();
	// ��ʼ����Ϸ״̬
	void Init();
	// ��Ϸѭ��
	void processInput(float dt);
	void update(float dt);
	void render();
	// ����ײ������
	void doCheckCollistion();
	void resetLevel();
	void resetPlayer();
};

#endif
