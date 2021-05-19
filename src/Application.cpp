/**
* Docs: https://docs.gl
*/
//#define FULLSCREEN
//#define DO_PROFILING

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "Camera.hpp"
#include "Debugging.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

#ifdef DO_PROFILING
#include "profiler.hpp"
#else
#define PROFILING_SCOPE(x)
#define PROFILE_FUNCTION()
#endif

float screen_width	= 1000;
float screen_height = 1000;
bool  _Close		= false;

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
Camera camera(glm::vec3(1.0f, 1.0f, 2.0f));

// variable for calculating cameraposition and speed
float deltaTime		= 0.0f; // time elapsed since last frame
float lastFrameTime = 0.0f; // last absolute frametime

// move camera left / right and foreward / backward
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	PROFILE_FUNCTION();

	glm::vec3 translation(0.0f, 0.0f, 0.0f);

	if (action == GLFW_REPEAT || action == GLFW_PRESS) {

		// camera controls
		// Foreward and Backward
		if (key == GLFW_KEY_E) {
			translation += camera.getFrontSpeed() * deltaTime;
		} else if (key == GLFW_KEY_R) {
			translation += camera.getFrontSpeed() * -deltaTime;
		}

		// Zoom
		if (key == GLFW_KEY_W) {
			translation += camera.getUpSpeed() * -deltaTime;
		} else if (key == GLFW_KEY_S) {
			translation += camera.getUpSpeed() * deltaTime;
		}

		// Left and Right
		if (key == GLFW_KEY_A) {
			translation += camera.getRightSpeed() * deltaTime;
		} else if (key == GLFW_KEY_D) {
			translation += camera.getRightSpeed() * -deltaTime;
		}

		// and apply the movement
		view = camera.move(translation);

		// close windows
		if (key == GLFW_KEY_ESCAPE) {
			_Close = true;
			return;
		}
	}
}

// recalculate the perspective matrix from the new screen size and update the drawing viewport
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	PROFILE_FUNCTION();

	glViewport(0, 0, width, height);
	screen_height = height;
	screen_width  = width;
	proj		  = camera.getPerspective(screen_width, screen_height);
}

int main() {

	//Instrumentor::Get().BeginSession("openGL");

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit())
		return -1;

// Create a windowed or fullscreen mode window and its OpenGL context
#ifdef FULLSCREEN
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	window				 = glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
#else
	window = glfwCreateWindow(screen_width, screen_height, "WOOOO HOOOO", NULL, NULL);
#endif

	// if window creation failed stop everything
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// callback for window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// callback for key pressed
	glfwSetKeyCallback(window, key_callback);

	// Make the window's context current, draw on this window
	glfwMakeContextCurrent(window);

	// set the swap interval as the v-sync
	glfwSwapInterval(1);

	// initializiong Glew
	if (glewInit() != GLEW_OK) {
		std::cout << "Error calling Glewinit";
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// enabling aplha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	// enable depth buffer
	// GLCall(glEnable(GL_DEPTH_TEST));

	// scope every gl buffer, this way they will automatically destroyed
	{

		PROFILING_SCOPE("buffers and co");

		// a cube
		float pos[] = {
			-0.5f, -0.5f, 0.0f, // 0 left down
			0.5f, -0.5f, 0.0f,	// 1 right down
			0.5f, 0.5f, 0.0f,	// 2 right up
			-0.5f, 0.5f, 0.0f,	// 3 left up

			1.5f, -0.5f, 0.0f, // 0 left down
			2.5f, -0.5f, 0.0f, // 1 right down
			2.5f, 0.5f, 0.0f,  // 2 right up
			1.5f, 0.5f, 0.0f,  // 3 left up
		};

		// take care of bind/unbinding and drawing
		Renderer renderer;

		VertexArray	 va;
		VertexBuffer vb(pos, sizeof(pos)); //5 * 24 * sizeof(float));

		VertexLayout layout;
		// 2 bytes for position, 2 for texture coordinates
		layout.Push(3, GL_FLOAT);
		// layout.Push(2, GL_FLOAT);
		va.AddBuffer(vb, layout);

		// using the vertices to create a square
		unsigned int tr_i[12] = {
			0, 1, 2, 2, 3, 0, // square one
			4, 5, 6, 6, 7, 4  // square two
		};
		IndexBuffer ib(tr_i, 12);

		proj = camera.getPerspective(screen_width, screen_width);
		//view = camera.point(0, -1);

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		Renderer::BindShaderProgram(shade);
		shade.SetUniform4f("u_Color", 0.3f, 0.4f, 0.5f, 1.0f);

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();

		// bind the used stuff
		Renderer::BindVertexArray(va);
		Renderer::BindIndexBuffer(ib);
		Renderer::BindShaderProgram(shade);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window) && !_Close) {

			PROFILING_SCOPE("draw loop");

			// update delta time for camera movement
			updateDTime();

			// Render here
			renderer.Clear();

			{
				PROFILING_SCOPE("MVP and uniform");

				MVP = proj * view * model;
				shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix
			}

			renderer.Draw(va, ib, shade);

			{
				PROFILING_SCOPE("swap buffers");
				// Swap front and back buffers
				// show front buffer, work on back buffer
				glfwSwapBuffers(window);

				// Poll for and process events
				glfwPollEvents();
			}
		}

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();
	}
	//Instrumentor::Get().EndSession();
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