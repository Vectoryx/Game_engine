/**
* Docs: https://docs.gl
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
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

#define FULLSCREEN

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(void) {
	GLFWwindow* window;
	// Initialize the library 
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context 
	#ifdef FULLSCREEN
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
	#else
	window = glfwCreateWindow(800, 800, "WOOOO HOOOO", NULL, NULL);
	#endif 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!window) {
		glfwTerminate();
		return -1;
	}


	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(10);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error calling Glewinit";
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float pos[] = {
			// vertex x, vertx y, textcoor x, textcoor y
			-1.0f, -1.0f, 0.0f, 0.0f, // 0 down left
			 1.0f, -1.0f, 1.0f, 0.0f, // 1 down right
			 1.0f,  1.0f, 1.0f, 1.0f, // 2 up left
			-1.0f,  1.0f, 0.0f, 1.0f, // 3 up right
			 0.0f,  0.0f, 0.5f, 0.5f  // 4 center
		};

		Renderer renderer;

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(pos, 5 * 4 * sizeof(float));

		VertexLayout layout;
		// 2 bytes for position, 2 for texture coordinates
		layout.Push(2, GL_FLOAT);
		layout.Push(2, GL_FLOAT);
		va.AddBuffer(vb, layout);

		unsigned int tr_i[6] = {0, 1, 2, 2, 3, 0};
		IndexBuffer ib(tr_i, 6);

		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		// shader.SetUniformMat4f("u_MVP", proj);
		Texture texture("res/textures/t1.png");
		Renderer::BindShaderProgram(shade);
		shade.SetUniform1i("u_Texture", 0);

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
			//shade.SetUniform4f("u_Color", 0.1f, 0.2f, 0.3f, 1.0f);

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