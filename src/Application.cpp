/**
* Docs: https://docs.gl
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "Camera.h"
#include "Debugging.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//#define FULLSCREEN

float screen_width	= 1000;
float screen_height = 1000;

void printMat4(glm::mat4 mat);
void rotateMat4(glm::mat4 &matrix, glm::vec3 amountDeg);
void translateMat4(glm::mat4 &matrix, glm::vec3 amout);

void updateDTime();

// MVP rojections
glm::mat4 proj(1.0f);
glm::mat4 view(1.0f);
glm::mat4 model(1.0f);
glm::mat4 MVP(1.0f);

// camera object
Camera camera(glm::vec3(2.0f, 0.5f, 0.0f));

float	  deltaTime		= 0.0f; // time elapsed since last frame
float	  lastFrameTime = 0.0f; // last absolute frametime
glm::vec2 mouse(-1.0f, -1.0f);

// move camera left / right and foreward / backward
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	glm::vec3 translation(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation(0.0f, 0.0f, 0.0f);

	model = glm::mat4(1.0f);

	if (action == GLFW_REPEAT || action == GLFW_PRESS) {

		// camera controls
		if (key == GLFW_KEY_A) {
			translation += camera.getRightSpeed() * -deltaTime;
		} else if (key == GLFW_KEY_D) {
			translation += camera.getRightSpeed() * deltaTime;
		}

		if (key == GLFW_KEY_W) {
			translation += camera.getFrontSpeed() * deltaTime;
		} else if (key == GLFW_KEY_S) {
			translation += camera.getFrontSpeed() * -deltaTime;
		}

		view = camera.move(translation);

		if (key == GLFW_KEY_C) {
			camera.zoom(-1);
			proj = camera.getPerspective(screen_width, screen_height);
		} else if (key == GLFW_KEY_X) {
			camera.zoom(1);
			proj = camera.getPerspective(screen_width, screen_height);
		}

		// rotation control
		if (key == GLFW_KEY_UP) {
			rotation.x -= 1.0f;
		} else if (key == GLFW_KEY_DOWN) {
			rotation.x += 1.0f;
		}

		if (key == GLFW_KEY_LEFT) {
			rotation.y -= 1.0f;
		} else if (key == GLFW_KEY_RIGHT) {
			rotation.y += 1.0f;
		}

		if (key == GLFW_KEY_PERIOD) {
			rotation.z -= 1.0f;
		} else if (key == GLFW_KEY_COMMA) {
			rotation.z += 1.0f;
		}
		//rotateMat4(model, rotation);

		if (key == GLFW_KEY_ESCAPE) {
			glfwTerminate();
			exit(0);
		}
	}
}

// recalculate the perspective matrix from the new screen size and update the drawing "canvas"
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	screen_height = height;
	screen_width  = width;
	proj		  = camera.getPerspective(screen_width, screen_height);
}

// make the camera look to the mouse
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

	if (mouse == glm::vec2(-1.0f, -1.0f)) {
		mouse.x = xpos;
		mouse.y = ypos;
	}

	view = camera.point(xpos - mouse.x, mouse.y - ypos);

	mouse = glm::vec2(xpos, ypos);
}

int main(void) {
	GLFWwindow *window;
	// Initialize the library
	if (!glfwInit())
		return -1;

// Create a windowed mode window and its OpenGL context
#ifdef FULLSCREEN
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	window				 = glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
#else
	window = glfwCreateWindow(screen_width, screen_height, "WOOOO HOOOO", NULL, NULL);
#endif

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "Error calling Glewinit";
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// enabling aplha
	//GLCall(glEnable(GL_BLEND));
	//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA));
	// enable depth buffer
	GLCall(glEnable(GL_DEPTH_TEST));

	{

		float pos[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0 left down
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	 // 1 right down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	 // 2 right up
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,	 // 3 left up

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // 0 left down
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	// 1 right down
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,	// 2 right up
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,	// 3 left up

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	 // 0 left down
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	 // 1 right down
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 2 right up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	 // 3 left up

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	// 0 left down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	// 1 right down
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 2 right up
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	// 3 left up

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 0 left down
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,	 // 1 right down
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	 // 2 right up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	 // 3 left up

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // 0 left down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	// 1 right down
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	// 2 right up
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,	// 3 left up
		};

		Renderer renderer;

		VertexArray	 va;
		VertexBuffer vb(pos, sizeof(pos)); //5 * 24 * sizeof(float));

		VertexLayout layout;
		// 2 bytes for position, 2 for texture coordinates
		layout.Push(3, GL_FLOAT);
		layout.Push(2, GL_FLOAT);
		va.AddBuffer(vb, layout);

		// using the vertices to create a square
		unsigned int tr_i[36] = {
			0, 1, 2, 2, 3, 0,		// square one
			4, 5, 6, 6, 7, 4,		// square two
			8, 9, 10, 10, 11, 8,	// square three
			12, 13, 14, 14, 15, 12, // square four
			16, 17, 18, 18, 19, 16, // square five
			20, 21, 22, 22, 23, 20	// square six
		};
		IndexBuffer ib(tr_i, 36);

		proj = camera.getPerspective(screen_width, screen_width);
		view = camera.point(0, -1);

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		Renderer::BindShaderProgram(shade);
		shade.SetUniform1i("u_Texture", 0); // use the texture;

		Texture texture("res/textures/this_is_snake.jpg");

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();

		Renderer::BindTexture(texture, 0);
		Renderer::BindVertexArray(va);
		Renderer::BindIndexBuffer(ib);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window)) {

			updateDTime();

			// Render here
			renderer.Clear();

			Renderer::BindShaderProgram(shade);
			MVP = proj * view * model;
			shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix

			renderer.Draw(va, ib, shade);

			// Swap front and back buffers
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}

		Renderer::UnBindShaderProgram();
	}
	glfwTerminate();
	return 0;
}

// for debug, print the matrix on console
void printMat4(glm::mat4 matrix) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("% 4.8f ", matrix[i][j]);
		}
		std::cout << std::endl;
	}
}

void translateMat4(glm::mat4 &matrix, glm::vec3 amount) {
	matrix = glm::translate(matrix, glm::vec3(amount.x, amount.y, amount.z));
}

void rotateMat4(glm::mat4 &matrix, glm::vec3 amountDeg) {
	matrix = glm::rotate(matrix, glm::radians(amountDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(amountDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(amountDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void updateDTime() {

	// framerate dependent way of measuring time
	float currentFrame = glfwGetTime();
	deltaTime		   = currentFrame - lastFrameTime;
	lastFrameTime	   = currentFrame;
}