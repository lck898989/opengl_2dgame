#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <string>
#include <vector>
#include "shader.h"
#include <glad/glad.h>
using namespace std;
typedef unsigned int uint;
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};
struct Texture {
	uint id;
	string type;
	aiString path;
};
class Mesh {
public:
	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
	void Draw(Shader shader);
private:
	uint VAO, VBO, EBO;
	void setupMesh();
};
#endif

