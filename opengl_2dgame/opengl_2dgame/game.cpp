#include "game.h"
#include "ballobject.h"
#include "particleGenerator.h"
#include "postProcessor.h"
#include <irrKlang.h>
#include <ft2build.h>
#include "myfont.h"
using namespace irrklang;
const glm::vec2 PLAYER_SIZE(100, 20);
// ��İ뾶
const float BALL_RADIUS = 12.5f;
// ��ʼ������ٶ�
const glm::vec2 INITIAL_BALL_VELOCITY(90.0f,-180.0f);

const float PLAYER_VELOCITY(500.0f);
// ���ӵ�����
uint particleCount = 500;
// �������
GameObject* player;
// Ӣ������
BallObject* hero;
// ����������
ParticleGenerator* particles;
// ���ڴ������
PostProcessor* effects;
// ����ʱ��
float shakeTime = 0.0f;
// ����
MyFont* font;
// ��Ƶ���� ʹ�õ��������ֲ�����
irrklang::ISoundEngine* engine = nullptr;
// ������ʼ��
Game::Game(uint width,uint height) {
	this->width = width;
	this->height = height;
	this->state = GAME_ACTIVE;
}
Game::Game() {

}
// ��������
Game::~Game() {
	// �Ƴ����е���Ƶ��Դ
	// engine->removeAllSoundSources();
	delete engine;
	engine = nullptr;
}

// �����Ļص�����
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
	// �����򷵻س�ȥ
	return (Direction)direction;
}
// �����ײ�ķ���AABB��ײ��������Բ�κͷ��ε���ײ��ⷽ��
bool checkCollision(GameObject &obj1,GameObject &obj2) {
	bool collisionX, collisionY;
	collisionX = collisionY = false;
	// x�������Ƿ���ײ��
	collisionX = obj1.position.x + obj1.size.x >= obj2.position.x && obj2.position.x + obj2.size.x >= obj1.position.x;
	// y�������Ƿ���ײ������
	collisionY = obj2.position.y + obj2.size.y >= obj1.position.y && obj1.position.y + obj1.size.y >= obj2.position.y;
	return collisionX && collisionY;
}
// ��������
CollisionInfo checkCollision(BallObject &ball,GameObject &block) {
	// ��ȡԲ������
	glm::vec2 circle_center(ball.position + ball.radius);
	// ����AABB����Ϣ
	glm::vec2 aabb_half_extents(block.size.x / 2, block.size.y / 2);
	// �ı��ε����ĵ�����
	glm::vec2 block_center(block.position.x + aabb_half_extents.x,block.position.y + aabb_half_extents.y);
	// ����Բ�����ĵ��ı��ε�ʸ����
	glm::vec2 c_to_bVec = circle_center - block_center;
	glm::vec2 m = max(c_to_bVec, -aabb_half_extents);
	glm::vec2 mi = min(m, aabb_half_extents);
	// ������������
	glm::vec2 clamped = glm::clamp(c_to_bVec,-aabb_half_extents,aabb_half_extents);
	// �����ı����ϵ�����㵽Բ�ĵ�����������
	glm::vec2 closet = block_center + clamped;
	// ��̾���
	c_to_bVec = closet - circle_center;
	if (glm::length(c_to_bVec) <= ball.radius) {
		Direction tempDir = getDirection(c_to_bVec);
		return make_tuple(true,tempDir,c_to_bVec);
	}
	else {
		return make_tuple(false, UP, glm::vec2(0.0f,0.0f));
	}
}

// ��ʼ����Ϸ״̬
void Game::Init() {
	
	engine = irrklang::createIrrKlangDevice();
	// ���ű�������da
	engine->play2D("audio/breakout.mp3",true);
	// ������ɫ��
	ResManager::loadShader("s.vs","s.fs",nullptr,"sprite");
	// ����������ص���ɫ��
	ResManager::loadShader("p.vs","p.fs",nullptr,"particle");
	// ��Ч��ɫ��
	ResManager::loadShader("pt.vs","pt.fs",nullptr,"post");
	// ������ɫ��
	ResManager::loadShader("font.vs","font.fs",nullptr,"font"); 
	// ������ɫ��
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),static_cast<float>(this->height),0.0f,-1.0f,1.0f);
	Shader spriteShader = ResManager::getShaderByName("sprite");
	// ����������ɫ��
	spriteShader.use().setInt("tex",0);
	spriteShader.use().setMat4("projection",projection);
	

	Shader particleShader = ResManager::getShaderByName("particle");
	particleShader.use().setInt("tex",0);
	particleShader.use().setMat4("projection",projection);

	Shader postShader = ResManager::getShaderByName("post");
	//postShader.setInt("tex",0);
	// ����ջ����Ķ�������д��ݸ�һ����������һ��Ҫע�⣬fontShader��ʱ�Ѿ��������ˣ��ں�����ʹ����һ�����������
	Shader fontShader = ResManager::getShaderByName("font");
	// ��ʼ������
	font = new MyFont(fontShader);

	effects = new PostProcessor(postShader,this->width,this->height);
	effects->chaos = false;
	effects->confuse = false;
	// ��ǰ�ؿ�Ϊ1
	this->level = 1;
	this->sRenderer = new SpriteRenderer(spriteShader);
	//SpriteRenderer sr = SpriteRenderer(spriteShader);
	// ��������      
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
	// ���عؿ�
	GameLevel one, two, three, four;
	one.load("level/one.txt",this->width,this->height * 0.5);
	two.load("level/two.txt",this->width,this->height * 0.5);
	three.load("level/three.txt",this->width,this->height * 0.5);
	four.load("level/four.txt",this->width,this->height * 0.5);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	// ��ʼ������ϵͳ
	particles = new ParticleGenerator(ResManager::getShaderByName("particle"),ResManager::getTextureByName("particle"), particleCount);
	
}
// ��Ϸѭ��
void Game::processInput(float dt) {
	if (this->state == GAME_ACTIVE) {
		float velocity = PLAYER_VELOCITY * dt;
		// �ƶ�����
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
		// �ո���ƶ�Ӣ��
		if (this->keys[GLFW_KEY_SPACE]) {
			hero->stuck = false;
		}
	}
}
void Game::update(float dt) {
	// ��ÿһ֡�����ƶ����ǵ��߼�
	hero->move(dt,this->width);
	// ������ײ���
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
	// �������е�ש�������״̬
	for (GameObject& item : this->levels[this->level].bricks) {
		if (item.isDestroy) {
			item.isDestroy = false;
		}
	}
}
void Game::resetPlayer() {
	// ���õ����λ��
	player->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - 2 * PLAYER_SIZE.y);
	// ����Ӣ�۵�λ��
	hero->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - 2 * PLAYER_SIZE.y) + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
}
void Game::render() {

	if (this->state == GAME_ACTIVE) {
		
		// ������Ч����֡���壬�����ɫ���壩
		effects->beginRender();
		// ������Ⱦ����
		Texture2D texture = ResManager::getTextureByName("background");
		// ���Ʊ���
		this->sRenderer->drawSprite(texture,glm::vec2(0,0),glm::vec2(this->width,this->height));
		// ���ƹؿ�
		this->levels[this->level].draw(*this->sRenderer);
		// ���Ƶ���
		player->draw(*this->sRenderer);
		// ����Ӣ������
		hero->draw(*this->sRenderer);
		// ��������
		particles->draw();
		// ������������
		font->draw("hello everybody", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
		// ������Ⱦ
		effects->endRender();
		effects->render(glfwGetTime());
		//font->draw("hello",25.0f,25.0f,1.0f,glm::vec3(0.5f,0.8f,0.2f));
	}
	/*string name = "face";
	Texture2D texture = ResManager::getTextureByName(name);
	this->sRenderer->drawSprite(texture,glm::vec2(200,200),glm::vec2(300,400),45.0f,glm::vec3(0.0f,1.0f,1.0f));*/
}
// �����ײ����
void Game::doCheckCollistion() {
	// �������ש�����ײ��ϵ
	for (GameObject &item : this->levels[this->level].bricks) {
		if (!item.isDestroy) {
			CollisionInfo info = checkCollision(*hero,item);
			// �����ײ�Ļ�������Ӧ�Ķ���
			if (get<0>(info)) {
				if (!item.isSolid) {
					item.isDestroy = true;
					// ����������ʵ�ĵ���Ƶ
					engine->play2D("audio/bleep.mp3");
				}
				else {
					shakeTime = 0.23f;
					effects->shake = true;
					// ������ײ��Ƶ
					engine->play2D("audio/solid.wav");
				}
				// ��ȡ�������
				glm::vec2 diff_vector = get<2>(info);
				Direction dir = get<1>(info);
				// ���ݷ�������Ӧ����
				if (dir == LEFT || dir == RIGHT) {
					// ��x������ٶȷ���
					hero->velocity.x = -hero->velocity.x;
					// �ض�λ
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
	// ������뵲�����ײ
	CollisionInfo res = checkCollision(*hero,*player);
	if (!hero->stuck && get<0>(res)) {
		engine->play2D("audio/bleep.wav");
		// �������ĵ������
		float centerBoard = player->position.x + player->size.x / 2;
		// �����ĵ������
		float distance = (hero->position.x + hero->size.x / 2) - centerBoard;
		// ���������ĵ�ľ����뵲��һ�����İٷֱ�
		float percentage = distance / (player->size.x / 2);
		float strength = 2.0f;
		glm::vec2 oldVelocity = hero->velocity;
		hero->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		hero->velocity.y = -oldVelocity.y;
		hero->velocity = glm::normalize(hero->velocity) * glm::length(oldVelocity);
	}
}

