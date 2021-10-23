
#include<iostream>
#include<glad/glad.h>

#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


const unsigned int width = 800;
const unsigned int height = 800;

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat distance = -5.0f;
GLfloat direction = 0.0f;


// DRAWING TRIANGLES
GLfloat vertices[] =
{	//	COORDINATES			/	 COLORS		//   HOW MAP THE PICTURE?
	/* -0.5f, 0.0f, 0.5f,		 0.83f, 0.70f, 0.44f,	0.0f, 0.0f,		// Lower left corner
	 -0.5f, 0.0f, -0.5f,	 0.83f, 0.70f, 0.44f,	5.0f, 0.0f,		// Lower right corner
	  0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,		// Upper right corner
	  0.5f, 0.0f, 0.5f,	     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,		
	  0.0f, 0.8f, 0.0f,	     0.92f, 0.86f, 0.76f,	2.5f, 5.0f	*/
		-0.5f, -0.5f,  0.5f, 		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,  
		 0.5f, -0.5f,  0.5f,		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,		0.83f, 0.70f, 0.44f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,		0.92f, 0.86f, 0.76f,	1.0f, 1.0f

};

GLuint indices[] = {
		2, 6, 7,
		2, 3, 7,

		//Bottom
		0, 4, 5,
		0, 1, 5,

		//Left
		0, 2, 6,
		0, 4, 6,

		//Right
		1, 3, 7,
		1, 5, 7,

		//Front
		0, 2, 3,
		0, 1, 3,

		//Back
		4, 6, 7,
		4, 5, 7

};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << key << std::endl;

	const GLfloat rotationSpeed = 10;
	const GLfloat dis = 0.5f;
	const GLfloat dir = 0.5f;

	// actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			rotationX -= rotationSpeed;
			break;
		case GLFW_KEY_DOWN:
			rotationX += rotationSpeed;
			break;
		case GLFW_KEY_RIGHT:
			rotationY += rotationSpeed;
			break;
		case GLFW_KEY_LEFT:
			rotationY -= rotationSpeed;
			break;

		case GLFW_KEY_W:
			distance += dis;
			break;

		case GLFW_KEY_S:
			distance -= dis;
			break;
		case GLFW_KEY_A:
			direction -= dir;
			break;
		case GLFW_KEY_D:
			direction += dir;
			break;
		}



	}
}


int main() {
	//Initialize GLFW
	glfwInit();
	//Which version I am using (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW I am using the CORE profile, it means that I have only modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//Create a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(width, height, "First Project", NULL, NULL);
	//Checking (error) if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, keyCallback);
	//Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//load GLAD so it configures OpenGL
	gladLoadGL();

	//Specify the viewport of OpenGL in the Window x(0,800), y(0,800)
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture
	Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			rotation += 0.5f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(direction, 0.0f, distance));
		proj = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.0f);
		
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));



		glUniform1f(uniID, 0.9f);
		popCat.Bind();
		//Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		//Take care of all GLFW events
		glfwPollEvents();
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	popCat.Delete();
	//Delete window before ending the program
	glfwDestroyWindow(window);
	//Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

