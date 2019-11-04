#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "shader.h"
#include "model.h"
#include "stb_image.h"
using namespace std;
typedef unsigned int uint;
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initGlfwWindow();
glm::mat4 createViewMatrix();
void initCube();
void initPlane();
uint loadTexture(char const *path);
void drawFloor(glm::mat4 model, Shader cubeShader);
void drawTwoBox(glm::mat4 model, Shader cubeShader);
void drawTwoScaledBox(glm::mat4 model, Shader cubeShader);

glm::mat4 projection = glm::mat4(1.0);
glm::mat4 view = glm::mat4(1.0);
// 摄像机的位置
glm::vec3 cameraPos = glm::vec3(0.0f,-5.0f,8.0f);
// 摄像机的方向
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
// 摄像机的up
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};
uint cubeVAO, cubeVBO, planeVAO, planeVBO,textureId,planeTexture;
int main() {
	// 初始化窗口
	GLFWwindow* window = initGlfwWindow();
	// 启用深度检测
	
	/*
		模型加载相关
		Shader modelShader = Shader("model.vs","model.fs");
		// 模型资源文件夹
		string path = "assets/nanosuit.obj";
		Model modelItem = Model(path);
		modelShader.use();
	
	*/
	view = createViewMatrix();
	Shader cubeShader = Shader("cube.vs","cube.fs");
	Shader cubeScaleShader = Shader("cubeBig.vs","cubeBig.fs");
	string cubePath = "container2.png";
	string planePath = "wall.jpg";
	textureId = loadTexture(cubePath.c_str());
	planeTexture = loadTexture(planePath.c_str());
	initCube();
	initPlane();
	cubeShader.use();
	projection = glm::perspective(glm::radians(-45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	cubeShader.setMat4("view", view);
	cubeShader.setMat4("projection", projection);
	
	//glActiveTexture(GL_TEXTURE0);
	cubeShader.setInt("tex", 0);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		// 帧缓冲中的颜色缓冲，深度缓冲，模板缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
		//glStencilMask(0x00);
		glm::mat4 model = glm::mat4(1.0);
		cubeShader.use();
		drawFloor(model,cubeShader);
		glStencilFunc(GL_ALWAYS,1,0xff);
		glStencilMask(0xff);
		drawTwoBox(model,cubeShader);
		glStencilFunc(GL_NOTEQUAL,1,0xff);
		// 禁止模板缓冲的写入
		glStencilMask(0x00);
		// 禁用深度测试
		glDisable(GL_DEPTH_TEST);
		cubeScaleShader.use();
		drawTwoScaledBox(model, cubeScaleShader);
		glStencilMask(0xff);
		glEnable(GL_DEPTH_TEST);
		/*glm::mat4 model = glm::mat4(1.0f);
		cubeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		/*
		
			加载模型相关代码
			view = createViewMatrix();
			projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
			modelShader.setMat4("view",view);
			modelShader.setMat4("projection",projection);
			glm::mat4 modelMatrix = glm::mat4(1.0);
			modelMatrix = glm::translate(modelMatrix,glm::vec3(0.0f,-2.0f,0.0f));
			float time = (float)glfwGetTime();
			modelMatrix = glm::rotate(modelMatrix,time,glm::vec3(0.0f,1.0f,0.0f));
			modelShader.setMat4("model",modelMatrix);
			modelItem.Draw(modelShader);
		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1,&cubeVAO);
	glDeleteVertexArrays(1,&planeVAO);
	glDeleteBuffers(1,&planeVBO);
	glDeleteBuffers(1,&cubeVBO);
	glfwTerminate();
	return 0;
}
void drawFloor(glm::mat4 model,Shader cubeShader) {
	glBindVertexArray(planeVAO);
	glActiveTexture(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, planeTexture);
	cubeShader.setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawTwoBox(glm::mat4 model, Shader cubeShader) {
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
	cubeShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
	cubeShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void drawTwoScaledBox(glm::mat4 model, Shader cubeShader) {
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f));
	model = glm::scale(model,glm::vec3(1.1f));
	cubeShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.1f));
	cubeShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
uint loadTexture(char const *path) {
	uint textureId;
	glGenTextures(1,&textureId);
	int width, height, channels;
	unsigned char* data = stbi_load(path,&width,&height,&channels,0);
	if (data) {
		GLenum format;
		if (channels == 1) {
			format = GL_RED;
		}
		else if (channels == 3) {
			format = GL_RGB;
		}
		else if (channels == 4) {
			format = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D,textureId);
		glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		stbi_image_free(data);

	}
	else {
		cout << "加载错误" << endl;
		stbi_image_free(data);
	}
	return textureId;
}
// 初始化箱子
void initCube() {
	//glGenBuffers(GL_ARRAY_BUFFER,&cubeVAO);
	glGenVertexArrays(1,&cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// 绑定buffer
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),&cubeVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

	glBindVertexArray(0);
}

// 初始化地板
void initPlane() {
	glGenVertexArrays(1,&planeVAO);
	glGenBuffers(1,&planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER,planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glBindVertexArray(planeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
glm::mat4 createViewMatrix() {
	glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);
	return view;
}
GLFWwindow* initGlfwWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "opengl game", NULL, NULL);
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