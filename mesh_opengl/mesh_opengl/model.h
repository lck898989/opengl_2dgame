#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <string>
#include "shader.h"
#include "mesh.h"
#include <vector>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <glad/glad.h>
using namespace std;
class Model {
public:
	Model(string path);
	Model();
	void Draw(Shader shader);
private:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	void loadModel(string path);
	void processNode(aiNode* node,const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	uint TextureFromFile(const char* path, const string &directory);
};
#endif
