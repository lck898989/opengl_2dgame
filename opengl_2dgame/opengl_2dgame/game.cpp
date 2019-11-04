#include "game.h"
#include "ballobject.h"
#include "particleGenerator.h"
#include "postProcessor.h"
#include <irrKlang.h>
#include <ft2build.h>
#include "myfont.h"
using namespace irrklang;
const glm::vec2 PLAYER_SIZE(100, 20);
// 球的半径
const float BALL_RADIUS = 12.5f;
// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(90.0f,-180.0f);

const float PLAYER_VELOCITY(500.0f);
// 粒子的数量
uint particleCount = 500;
// 挡板对象
GameObject* player;
// 英雄主角
BallObject* hero;
// 粒子生成器
ParticleGenerator* particles;
// 后期处理程序
PostProcessor* effects;
// 抖动时间
float shakeTime = 0.0f;
// 字体
MyFont* font;
// 音频引擎 使用第三方音乐播放器
irrklang::ISoundEngine* engine = nullptr;
// 字体库初始化
Game::Game(uint width,uint height) {
	this->width = width;
	this->height = height;
	this->state = GAME_ACTIVE;
}
Game::Game() {

}
// 析构函数
Game::~Game() {
	// 移除所有的音频资源
	// engine->removeAllSoundSources();
	delete engine;
	engine = nullptr;
}

// 获得球的回弹方向
Direction getDirection(glm::vec2 target) {
	glm::vec2 dirArr[] = {
		glm::vec2(0.0f,1.0f),
		glm::vec2(1.0f,0.0f),
		glm::vec2(0.0f,-1.0f),
		glm::vec2(-1.0f,0.0f)
	};
	float max = 0.0f;
	uint direction = -1;
	for (uint i = 0; i < 4; i++) {
		float dotRes = glm::dot(glm::normalize(target), dirArr[i]);
		if (max < dotRes) {
			max = dotRes;
			direction = i;
		}
	}
	// 将方向返回出去
	return (Direction)direction;
}
// 检测碰撞的方法AABB碰撞方法不是圆形和方形的碰撞检测方法
bool checkCollision(GameObject &obj1,GameObject &obj2) {
	bool collisionX, collisionY;
	collisionX = collisionY = false;
	// x方向上是否碰撞了
	collisionX = obj1.position.x + obj1.size.x >= obj2.position.x && obj2.position.x + obj2.size.x >= obj1.position.x;
	// y方向上是否碰撞产生了
	collisionY = obj2.position.y + obj2.size.y >= obj1.position.y && obj1.position.y + obj1.size.y >= obj2.position.y;
	return collisionX && collisionY;
}
// 函数重载
CollisionInfo checkCollision(BallObject &ball,GameObject &block) {
	// 获取圆的中心
	glm::vec2 circle_center(ball.position + ball.radius);
	// 计算AABB的信息
	glm::vec2 aabb_half_extents(block.size.x / 2, block.size.y / 2);
	// 四边形的中心点坐标
	glm::vec2 block_center(block.position.x + aabb_half_extents.x,block.position.y + aabb_half_extents.y);
	// 计算圆的中心到四边形的矢量差
	glm::vec2 c_to_bVec = circle_center - block_center;
	glm::vec2 m = max(c_to_bVec, -aabb_half_extents);
	glm::vec2 mi = min(m, aabb_half_extents);
	// 计算限制向量
	glm::vec2 clamped = glm::clamp(c_to_bVec,-aabb_half_extents,aabb_half_extents);
	// 计算四边形上的任意点到圆心的最近点的坐标
	glm::vec2 closet = block_center + clamped;
	// 最短距离
	c_to_bVec = closet - circle_center;
	if (glm::length(c_to_bVec) <= ball.radius) {
		Direction tempDir = getDirection(c_to_bVec);
		return make_tuple(true,tempDir,c_to_bVec);
	}
	else {
		return make_tuple(false, UP, glm::vec2(0.0f,0.0f));
	}
}

// 初始化游戏状态
void Game::Init() {
	
	engine = irrklang::createIrrKlangDevice();
	// 播放背景音乐da
	engine->play2D("audio/breakout.mp3",true);
	// 加载着色器
	ResManager::loadShader("s.vs","s.fs",nullptr,"sprite");
	// 加载粒子相关的着色器
	ResManager::loadShader("p.vs","p.fs",nullptr,"particle");
	// 特效着色器
	ResManager::loadShader("pt.vs","pt.fs",nullptr,"post");
	// 字体着色器
	ResManager::loadShader("font.vs","font.fs",nullptr,"font"); 
	// 配置着色器
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),static_cast<float>(this->height),0.0f,-1.0f,1.0f);
	Shader spriteShader = ResManager::getShaderByName("sprite");
	// 将纹理传入着色器
	spriteShader.use().setInt("tex",0);
	spriteShader.use().setMat4("projection",projection);
	

	Shader particleShader = ResManager::getShaderByName("particle");
	particleShader.use().setInt("tex",0);
	particleShader.use().setMat4("projection",projection);

	Shader postShader = ResManager::getShaderByName("post");
	//postShader.setInt("tex",0);
	// 存在栈里面的对象变量中传递给一个函数里面一定要注意，fontShader此时已经被回收了，在后续的使用中一定会出现问题
	Shader fontShader = ResManager::getShaderByName("font");
	// 初始化字体
	font = new MyFont(fontShader);

	effects = new PostProcessor(postShader,this->width,this->height);
	effects->chaos = false;
	effects->confuse = false;
	// 当前关卡为1
	this->level = 1;
	this->sRenderer = new SpriteRenderer(spriteShader);
	//SpriteRenderer sr = SpriteRenderer(spriteShader);
	// 加载纹理      
	ResManager::loadTexture("assets/face.png",true,"face");
	ResManager::loadTexture("assets/block.png",true,"block");
	ResManager::loadTexture("assets/block_solid.png",true,"block_solid");
	ResManager::loadTexture("assets/background.jpg",false,"background");
	ResManager::loadTexture("assets/paddle.png",true,"paddle");
	ResManager::loadTexture("assets/particle.png",true,"particle");
	glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2,this->height - 2 * PLAYER_SIZE.y);
	player = new GameObject(playerPos, PLAYER_SIZE, ResManager::getTextureByName("paddle"));

	// hero
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	hero = new BallObject(ballPos,BALL_RADIUS,INITIAL_BALL_VELOCITY,ResManager::getTextureByName("face"));
	// 加载关卡
	GameLevel one, two, three, four;
	one.load("level/one.txt",this->width,this->height * 0.5);
	two.load("level/two.txt",this->width,this->height * 0.5);
	three.load("level/three.txt",this->width,this->height * 0.5);
	four.load("level/four.txt",this->width,this->height * 0.5);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	// 初始化粒子系统
	particles = new ParticleGenerator(ResManager::getShaderByName("particle"),ResManager::getTextureByName("particle"), particleCount);
	
}
// 游戏循环
void Game::processInput(float dt) {
	if (this->state == GAME_ACTIVE) {
		float velocity = PLAYER_VELOCITY * dt;
		// 移动挡板
		if (this->keys[GLFW_KEY_A]) {
			if (player->position.x >= 0) {
				player->position.x -= velocity;
			}
			else {
				//cout << "player's position is " << player->position.x << endl;
			}
		}
		if (this->keys[GLFW_KEY_D]) {
			if (player->position.x <= (this->width - player->size.x)) {
				player->position.x += velocity;
			}
			else {
				//cout << "player's position is " << player->position.x << endl;
			}
		}
		// 空格键移动英雄
		if (this->keys[GLFW_KEY_SPACE]) {
			hero->stuck = false;
		}
	}
}
void Game::update(float dt) {
	// 在每一帧调用移动主角的逻辑
	hero->move(dt,this->width);
	// 处理碰撞检测
	this->doCheckCollistion();
	if (hero->position.y >= this->height) {
		this->resetLevel();
		this->resetPlayer();
	}
	bool isPass = this->levels[this->level].isComplete();
	if (isPass) {
		this->level++;
		this->resetPlayer();
	}
	if (shakeTime > 0.0f) {
		shakeTime -= dt;
		if (shakeTime <= 0.0f) {
			effects->shake = false;
		}
	}
	particles->update(dt,*hero,2,glm::vec2(hero->radius / 2));
}
void Game::resetLevel() {
	// 重置所有的砖块的销毁状态
	for (GameObject& item : this->levels[this->level].bricks) {
		if (item.isDestroy) {
			item.isDestroy = false;
		}
	}
}
void Game::resetPlayer() {
	// 重置挡板的位置
	player->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - 2 * PLAYER_SIZE.y);
	// 重置英雄的位置
	hero->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - 2 * PLAYER_SIZE.y) + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
}
void Game::render() {

	if (this->state == GAME_ACTIVE) {
		
		// 绘制特效（绑定帧缓冲，清除颜色缓冲）
		effects->beginRender();
		// 正常渲染过程
		Texture2D texture = ResManager::getTextureByName("background");
		// 绘制背景
		this->sRenderer->drawSprite(texture,glm::vec2(0,0),glm::vec2(this->width,this->height));
		// 绘制关卡
		this->levels[this->level].draw(*this->sRenderer);
		// 绘制挡板
		player->draw(*this->sRenderer);
		// 绘制英雄主角
		hero->draw(*this->sRenderer);
		// 绘制粒子
		particles->draw();
		// 绘制字体数据
		font->draw("hello everybody", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
		// 结束渲染
		effects->endRender();
		effects->render(glfwGetTime());
		//font->draw("hello",25.0f,25.0f,1.0f,glm::vec3(0.5f,0.8f,0.2f));
	}
	/*string name = "face";
	Texture2D texture = ResManager::getTextureByName(name);
	this->sRenderer->drawSprite(texture,glm::vec2(200,200),glm::vec2(300,400),45.0f,glm::vec3(0.0f,1.0f,1.0f));*/
}
// 检测碰撞计算
void Game::doCheckCollistion() {
	// 检测球与砖块的碰撞关系
	for (GameObject &item : this->levels[this->level].bricks) {
		if (!item.isDestroy) {
			CollisionInfo info = checkCollision(*hero,item);
			// 如果碰撞的话进行相应的动作
			if (get<0>(info)) {
				if (!item.isSolid) {
					item.isDestroy = true;
					// 播放碰到非实心的音频
					engine->play2D("audio/bleep.mp3");
				}
				else {
					shakeTime = 0.23f;
					effects->shake = true;
					// 播放碰撞音频
					engine->play2D("audio/solid.wav");
				}
				// 获取侵入深度
				glm::vec2 diff_vector = get<2>(info);
				Direction dir = get<1>(info);
				// 根据方向做相应动作
				if (dir == LEFT || dir == RIGHT) {
					// 将x方向的速度反向
					hero->velocity.x = -hero->velocity.x;
					// 重定位
					float offset = hero->radius - abs(diff_vector.x);
					if (dir == LEFT) {
						hero->position.x += offset;
					}
					else {
						hero->position.x -= offset;
					}
				}
				else {
					hero->velocity.y = -hero->velocity.y;
					float offset = hero->radius - abs(diff_vector.y);
					if (dir == UP) {
						hero->position.y -= offset;
					}
					else {
						hero->position.y += offset;
					}
				}
			}
		}
	}
	// 检测球与挡板的碰撞
	CollisionInfo res = checkCollision(*hero,*player);
	if (!hero->stuck && get<0>(res)) {
		engine->play2D("audio/bleep.wav");
		// 挡板中心点的坐标
		float centerBoard = player->position.x + player->size.x / 2;
		// 球到中心点的坐标
		float distance = (hero->position.x + hero->size.x / 2) - centerBoard;
		// 计算球到中心点的距离与挡板一半距离的百分比
		float percentage = distance / (player->size.x / 2);
		float strength = 2.0f;
		glm::vec2 oldVelocity = hero->velocity;
		hero->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		hero->velocity.y = -oldVelocity.y;
		hero->velocity = glm::normalize(hero->velocity) * glm::length(oldVelocity);
	}
}

