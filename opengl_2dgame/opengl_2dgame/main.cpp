#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <iostream>
#include "game.h"
#include "resourcemanager.h"

typedef unsigned int uint;
typedef unsigned char uchar;
const uint SCREEN_WIDTH = 800;
const uint SCREEN_HEIGHT = 600;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initGlfwWindow();
struct student {
	uint id;
	char name[64];
};
// 内存对齐规则
struct st {
	uint id;
	char sex;
	double salary;
};
typedef struct student structS;
void printstudent(structS& sp);
void prints(Shader s) {
	// 改变不了外面传进来的对象的值     
	s.ID = 8;
}

float deltaTime, lastFrame;
Game breakout;
map<string, Shader> shaders;
int main() {
	GLFWwindow* window = initGlfwWindow();
	breakout = Game(SCREEN_WIDTH, SCREEN_HEIGHT);
	breakout.Init();
	// 初始化游戏
	struct student s = { 1,'lck' };
	cout << "sizeof student is " << sizeof(struct student) << endl;
	cout << "sizeof s is " << sizeof(s) << endl;
	cout << "sizeof char is " << sizeof(char) << endl;
	cout << "sizeof st is " << sizeof(st) << endl;
	printstudent(s);
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		breakout.processInput(deltaTime);

		// Update Game state
		breakout.update(deltaTime);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		breakout.render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResManager::clear();
	glfwTerminate();
	return 0;
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;
}
void printstudent(structS& sp) {
	cout << sp.id << " " << sp.name << endl;
	const int a = 10;
	int *p;
	p = (int *)&a;
	*p = 20;
	cout << "a is " << a << endl;
	cout << "*p is " << *p << endl;

}
GLFWwindow* initGlfwWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "opengl 2D game", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window,key_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "failed to load glfw" << endl;
		return nullptr;
	}
	return window;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			breakout.keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			breakout.keys[key] = GL_FALSE;
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}