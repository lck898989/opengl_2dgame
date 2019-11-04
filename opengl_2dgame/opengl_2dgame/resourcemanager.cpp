#include "resourcemanager.h"
map<string, Texture2D> ResManager::textures;
map<string, Shader> ResManager::shaders;
Shader ResManager::loadShader(const char* vpath,const char* fpath,const char* gpath,string name) {
	shaders[name] = loadShaderFromFile(vpath,fpath,gpath);
	return shaders[name];
}
Texture2D ResManager::loadTexture(const char* path,bool alpha,string name) {
	textures[name] = loadTextureFromFile(path,alpha);
	return textures[name];
}
Texture2D ResManager::getTextureByName(string name) {
	return textures[name];
}
Shader ResManager::getShaderByName(string name) {
	return shaders[name];
}
Shader ResManager::loadShaderFromFile(const char* vpath, const char* fpath, const char* gpath) {
	string vertexCode, fragmentCode, gCode;
	ifstream vShaderFile, fShaderFile, gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// open file
		vShaderFile.open(vpath);
		fShaderFile.open(fpath);
		if (gpath != nullptr) {
			gShaderFile.open(gpath);
		}
		stringstream vShaderStream, fShaderStream, gShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		if (gpath != nullptr) {
			gShaderStream << gShaderFile.rdbuf();
		}
		vShaderFile.close();
		fShaderFile.close();
		if (gpath != nullptr) {
			gShaderFile.close();
		}
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (gpath != nullptr) {
			gCode = gShaderStream.str();
		}


	}
	catch (ifstream::failure e) {
		cout << "load shader file error" << endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = nullptr;
	if (gpath != nullptr) {
		const char* gShaderCode = gCode.c_str();
	}
	Shader shaderItem = Shader();
	shaderItem.compile(vShaderCode, fShaderCode, gpath != nullptr ? gShaderCode : nullptr);
	return shaderItem;
}
void ResManager::clear() {
	for (auto item : shaders) {
		glDeleteProgram(item.second.ID);
	}
	for (auto item : textures) {
		glDeleteTextures(1,&item.second.ID);
	}
}
Texture2D ResManager::loadTextureFromFile(const char* path,bool alpha) {
	Texture2D texture;
	if (alpha) {
		texture.format_image = GL_RGBA;
		texture.format_internal = GL_RGBA;
		texture.wrap_s = GL_MIRRORED_REPEAT;
		texture.wrap_t = GL_MIRRORED_REPEAT;
	}
	int width, height, channel;
	uchar* data = nullptr;
	data = stbi_load(path, &width, &height, &channel, 0);
	if (data) {
		texture.generate(width, height, data);
		stbi_image_free(data);
	}
	else {
		cout << "load image error" << endl;
		stbi_image_free(data);
	}
	return texture;
}