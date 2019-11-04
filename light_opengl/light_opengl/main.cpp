#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/GL.h>
#include <string>
#include <iostream>
#include "shader.h"
#include "stb_image.h"
using namespace std;
typedef unsigned int uint;
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initGlfwWindow();
void renderTriangle();
void drawArrays();
void renderBox();
void drawBox(uint vao,string type,Shader shaderItem);
glm::mat4 createViewMatrix();
// ��Ⱦ�ƹ�������
void renderLightBox();
void rotateLight();
void changeLightColor(Shader shaderProgram);
void loadImage(string url,uint index,string type);
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
float firstVertices[] = {
	1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
	-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,0.0f,1.0f

};
float lightTextVertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
uint VAO, VBO,lightVAO;
// �������λ��
glm::vec3 cameraPos = glm::vec3(-2.0f,0.0f,5.0f);
// ������ķ���
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
// �������up
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);


// �۲����
glm::mat4 viewMatrix = glm::mat4(1.0f);
// ͶӰ����
glm::mat4 projectionMatrix = glm::mat4(1.0f);

float fov;
glm::vec3 lightPos = glm::vec3(-1.0f, 1.0f, 1.0f);
uint texture0,texture1,texture2;
int main() {
	// ��ʼ������
	GLFWwindow* window = initGlfwWindow();
	// ������ȼ��
	glEnable(GL_DEPTH_TEST);
	// ͶӰ����
	projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	viewMatrix = createViewMatrix();
	loadImage("container2.png",0,"png");
	loadImage("container2_specular.png",1,"png");
	// ����shader
	Shader shaderProgram = Shader("box.vs","box.fs");
	//ShaderTest shadertest = ShaderTest("box.vs","box.fs");
	shaderProgram.use();
	renderBox();
	
	// ������ɫ
	glm::vec3 objectColor = glm::vec3(1.0f, 0.4f, 0.0f);
	glUniform3fv(glGetUniformLocation(shaderProgram.ID, "objectColor"), 1, glm::value_ptr(objectColor));
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	shaderProgram.setInt("material.diffuse", 0);
	shaderProgram.setInt("material.specular", 1);
	shaderProgram.setFloat("material.shininess",32.0f);
	shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 0.5f, 0.8f));
	shaderProgram.setVec3("light.ambient", glm::vec3(0.3f,0.3f,0.3f));
	shaderProgram.setVec3("light.diffuse", glm::vec3(0.5f,0.5f,0.5f));
	shaderProgram.setVec3("light.specular",glm::vec3(1.0f, 1.0f, 1.0f));
	shaderProgram.setVec3("viewPos",cameraPos);

	Shader lightShader = Shader("light.vs","light.fs");
	lightShader.use();
	//renderTriangle();
	renderLightBox();
	// �ƹ�uniform
	
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// �ı���յ�λ��
		rotateLight();
		/*
		

		 �����������shader


		*/
		shaderProgram.use();
		//changeLightColor(shaderProgram);
		glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightPos"), 1, glm::value_ptr(lightPos));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,texture1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,texture2);
		//shaderProgram.use();
		//drawArrays();
		drawBox(VAO,"box",shaderProgram);
		/*
		���õƹ����shader
		*/
		lightShader.use();
		glm::mat4 lightModelMatrix = glm::mat4(1.0f);
		lightModelMatrix = glm::translate(lightModelMatrix, lightPos);
		lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.5f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModelMatrix));
		drawBox(lightVAO,"light",lightShader);
		//shaderProgram.setMa
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindVertexArray(VAO);
	glfwTerminate();
	return 0;
}
//void setMatrix(glm::mat4 model,glm::mat4 view,glm::mat4 projection,Shader shaderProgram) {
//	
//}
void changeLightColor(Shader shaderProgram) {
	float time = (float)glfwGetTime();
	// �ƹ���ɫ
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightColor.x = sin(time * 2.0f);
	lightColor.y = sin(time * 1.0f);
	lightColor.z = sin(time * 2.8f);
	glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, glm::value_ptr(lightColor));
	glm::vec3 lightDiff = lightColor * glm::vec3(0.5f);
	glm::vec3 lightAmbient = lightDiff * glm::vec3(0.2f);
	//vec3 lightAmbient = ;
	shaderProgram.setVec3("light.ambient", lightAmbient);
	shaderProgram.setVec3("light.diffuse", lightDiff);
}
// ������������ͼ
void loadImage(string url,uint index,string type) {
	if (index == 0) {
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1,&texture0);
		glBindTexture(GL_TEXTURE_2D,texture0);
	}
	else if (index == 1) {
		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1,&texture1);
		glBindTexture(GL_TEXTURE_2D,texture1);
	}
	else if (index == 2) {
		glActiveTexture(GL_TEXTURE2);
		glGenTextures(1,&texture2);
		glBindTexture(GL_TEXTURE_2D,texture2);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channel;
	unsigned char *data = stbi_load(url.c_str(),&width,&height,&channel,0);
	if (data) {
		if (type == "png") {
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);

		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "load image error" << endl;
	}
	// �ͷ�����ռ�õ��ڴ�
	stbi_image_free(data);

}
void rotateLight() {
	float radius = 5.0f;
	float time = (float)glfwGetTime();
	float temx = sin(time) * radius;
	float temz = cos(time) * radius;
	lightPos.x = temx;
	lightPos.z = temz;
}
glm::mat4 createViewMatrix() {
	glm::mat4 view = glm::lookAt(cameraPos,cameraFront,cameraUp);
	return view;
}
void renderTriangle() {
	/*glGenBuffers(1, &VBO);*/
	// ���������������Կ�
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(firstVertices),firstVertices,GL_STATIC_DRAW);
	// ��������ָ��
	glBindVertexArray(VAO);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

}
void renderBox() {
	/*glGenBuffers(1, &VBO);*/
	// ���������������Կ�
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightTextVertices), lightTextVertices, GL_STATIC_DRAW);
	// ��������ָ��
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void renderLightBox() {
	glGenVertexArrays(1,&lightVAO);
	if (!VBO) {
		glGenBuffers(1,&VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(lightTextVertices), lightTextVertices,GL_STATIC_DRAW);
	}
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void drawBox(uint vao,string type,Shader shaderItem) {
	if (type != "box") {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else {
		glBindVertexArray(vao);
		for (uint i = 0; i < 10; i++) {
			glm::mat4 modelTemp = glm::mat4(1.0f);
			modelTemp = glm::translate(modelTemp,cubePositions[i]);
			float angles = (float)glfwGetTime() + i * 50;
			modelTemp = glm::rotate(modelTemp,angles,glm::vec3(1.0f,1.0f,0.0f));
			shaderItem.setMat4("model",modelTemp);
			glDrawArrays(GL_TRIANGLES,0,36);
			
		}
	}
}
void drawArrays() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,3);
}
GLFWwindow* initGlfwWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"opengl game", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "failed to load glfw" << endl;
		return nullptr;
	}
	return window;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}