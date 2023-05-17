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

#include "Debugging.h"
#include "renderer.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//#define FULLSCREEN

float screen_width = 1000;
float screen_height = 1000;

void print_matrix4(glm::mat4);
void rotateModel(glm::vec3 amountDeg);
void translateModel(glm::vec3 amout);

glm::vec3 translation(0.0f, 0.0f, 0.0f);
glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::mat4 proj(1.0f);
glm::mat4 MVP(1.0f);
glm::mat4 view(1.0f);
glm::mat4 model(1.0f);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	model = glm::mat4(1.0f);

	if (action == GLFW_REPEAT || action == GLFW_PRESS) {
		if (key == GLFW_KEY_RIGHT) {
			translation.x -= 0.1;
		} else if (key == GLFW_KEY_LEFT) {
			translation.x += 0.1;
		}

		if (key == GLFW_KEY_UP) {
			translation.y -= 0.1;
		} else if (key == GLFW_KEY_DOWN) {
			translation.y += 0.1;
		}

		translateModel(translation);

		if (key == GLFW_KEY_W) {
			rotation.x -= 1.0f;
		} else if (key == GLFW_KEY_S) {
			rotation.x += 1.0f;
		}

		if (key == GLFW_KEY_A) {
			rotation.y -= 1.0f;
		} else if (key == GLFW_KEY_D) {
			rotation.y += 1.0f;
		}

		if (key == GLFW_KEY_R) {
			rotation.z -= 1.0f;
		} else if (key == GLFW_KEY_E) {
			rotation.z += 1.0f;
		}
		rotateModel(rotation);

		MVP = proj * view * model;
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(void) {
	GLFWwindow *window;
	// Initialize the library
	if (!glfwInit())
		return -1;

// Create a windowed mode window and its OpenGL context
#ifdef FULLSCREEN
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
#else
	window = glfwCreateWindow(screen_width, screen_height, "WOOOO HOOOO", NULL, NULL);
#endif

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

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

	{

		/* 
		float pos[] = {
			// vertex x, vertx y, textcoor x, textcoor y
			0.0, 20.0f, 0.0f, 0.0f,	  // 0 left down
			20.0f, 20.0f, 1.0f, 0.0f, // 1 right down
			20.0f, 0.0f, 1.0f, 1.0f,  // 2 right up
			0.0f, 0.0f, 0.0f, 1.0f,	  // 3 left up
		}; */

		float pos[]{
			-0.5f, -0.5f, 0.0f, 0.0f, // 0 left down
			0.5f, -0.5f, 1.0f, 0.0f,  // 1 right down
			0.5f, 0.5f, 1.0f, 1.0f,	  // 2 right up
			-0.5f, 0.5f, 0.0f, 1.0f	  // 3 left up
		};

		float vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0 left down
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	 // 1 right down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	 // 2 right up
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,	 // 3 left up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	 // 0 left down
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	 // 1 right down
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,	 // 2 right up
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,	 // 3 left up
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	 // 0 left down
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	 // 1 right down
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 2 right up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	 // 3 left up
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	 // 0 left down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	 // 1 right down
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	 // 2 right up
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	 // 3 left up
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 0 left down
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,	 // 1 right down
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	 // 2 right up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	 // 3 left up
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,	 // 0 left down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	 // 1 right down
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	 // 2 right up
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,	 // 3 left up
		};

		Renderer renderer;

		// enabling aplha
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA));

		VertexArray	 va;
		VertexBuffer vb(pos, 4 * 4 * sizeof(float));

		VertexLayout layout;
		// 2 bytes for position, 2 for texture coordinates
		layout.Push(2, GL_FLOAT);
		layout.Push(2, GL_FLOAT);
		va.AddBuffer(vb, layout);

		// using the vertices to create a square
		unsigned int tr_i[6] = {0, 1, 2, 2, 3, 0};
		IndexBuffer	 ib(tr_i, 6);

		proj = glm::perspective(glm::radians(55.0f), float(screen_width / screen_height), 0.1f, 100.0f);

		view = glm::lookAt(glm::vec3(0.5f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		MVP = proj * view * model;

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		Renderer::BindShaderProgram(shade);
		shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix
		shade.SetUniform1i("u_Texture", 0);	 // use the texture;

		Texture texture("res/textures/this_is_snake.jpg");

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();
		Renderer::BindTexture(texture, 0);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window)) {

			// Render here
			renderer.Clear();

			Renderer::BindShaderProgram(shade);
			shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix

			Renderer::BindVertexArray(va);
			Renderer::BindIndexBuffer(ib);

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
void print_matrix4(glm::mat4 matrix) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("% 4.8f ", matrix[i][j]);
		}
		std::cout << std::endl;
	}
}

void translateModel(glm::vec3 amount) {
	model = glm::translate(model, glm::vec3(amount.x, amount.y, amount.z));
}

void rotateModel(glm::vec3 amountDeg) {
	model = glm::rotate(model, glm::radians(amountDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(amountDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(amountDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

//          1.0
//           │
//           │
//-1.0 ──────┼──────1.0
//           │
//           │
//         -1.0
//
//      │
//      V
//proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f); // set the drawing screen from top left

//      │
// 0.0  V
//  ┌──────1.0
//  │
//  │
// 1.0

//proj = glm::perspective(glm::radians(45.0f), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);