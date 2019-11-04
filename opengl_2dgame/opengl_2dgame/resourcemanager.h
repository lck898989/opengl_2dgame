#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <map>
#include "shader.h"
#include "texture.h"
#include <fstream>
#include <sstream>
using namespace std;
class ResManager {
public:
	static map<string, Texture2D> textures;
	static map<string, Shader> shaders;
	static Texture2D loadTexture(const char* path,bool alpha,string name);
	static Texture2D getTextureByName(string name);
	static Shader loadShader(const char* vpath, const char* fpath, const char* gpath,string name);
	static Shader getShaderByName(string name);
	static void clear();

private:
	ResManager() {}
	static Shader loadShaderFromFile(const char* vpath, const char* fpath, const char* gpath);
	static Texture2D loadTextureFromFile(const char* path,bool alpha);
};
#endif // !ResManager_H
