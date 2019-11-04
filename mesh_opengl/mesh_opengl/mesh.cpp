#include "mesh.h"
Mesh::Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}
void Mesh::Draw(Shader shader) {
	uint diffuseNr = 1;
	uint specularNr = 1;
	for (uint i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		// 获取纹理序号
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		/*shader.setFloat(("material." + name + number).c_str(), i);*/
		shader.setInt((name + number).c_str(),i);
		glBindTexture(GL_TEXTURE_2D,textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	// 绘制网格
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,&indices[0]);
	//glBindVertexArray(0);
}
void Mesh::setupMesh() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1,&VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)(offsetof(Vertex,normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}