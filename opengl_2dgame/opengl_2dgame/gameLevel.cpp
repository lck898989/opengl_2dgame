#include "gameLevel.h"
GameLevel::GameLevel() {

}
void GameLevel::load(const char* file,uint levelWidth,uint levelHeight) {
	// ��չ�������
	this->bricks.clear();
	// ���ļ��м���
	uint tileCode;
	GameLevel level;
	string line;
	ifstream fstream(file);
	vector<vector<uint>> tileData;
	if (fstream) {
		while (getline(fstream,line)) { // ��ȡ�ļ��е�ÿһ��
			istringstream sstream(line);
			vector<uint> row;
			while (sstream >> tileCode) { // ��ȡ���ո�ָ���ÿ������
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0) {
			this->init(tileData,levelWidth,levelHeight);
		}
	}
}
void GameLevel::draw(SpriteRenderer &renderer) {
	for (GameObject &tile : this->bricks) {
		if (!tile.isDestroy) {
			tile.draw(renderer);
		}
	}
}
// �Ƿ������Ϸ
bool GameLevel::isComplete() {
	for (GameObject &tile : this->bricks) {
		if (!tile.isDestroy && !tile.isSolid) {
			return false;
		}
	}
	return true;
}
void GameLevel::init(vector<vector<uint>> tileData,uint levelWidth,uint levelHeight) {
	uint width = tileData[0].size();
	uint height = tileData.size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / height;
	// ��ʼ���ؿ�
	for (uint y = 0; y < height; ++y) {
		for (uint x = 0; x < width; ++x) {
			/*if (x == 1) {
				break;
			}*/
			// ���ש������
			if (tileData[y][x] == 1) {
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResManager::getTextureByName("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.isSolid = GL_TRUE;
				this->bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				// ���ݾ���Ԫ�����ͼ��ز�ͬ��ͼƬ����ɫ
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2) {
					color = glm::vec3(0.2f,0.6,1.0f);
				}
				else if (tileData[y][x] == 3) {
					color = glm::vec3(0.0f,0.7f,0.0f);
				}
				else if (tileData[y][x] == 4) {
					color = glm::vec3(0.8f,0.8f,0.4f);
				}
				else if (tileData[y][x] == 5) {
					color = glm::vec3(1.0f,0.5f,0.0f);
				}
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->bricks.push_back(GameObject(pos,size,ResManager::getTextureByName("block"),color));
			}
		}
	}
}