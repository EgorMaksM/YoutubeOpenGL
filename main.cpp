#include "Model.h"


const unsigned int width = 800;
const unsigned int height = 800;


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(width, height, "My first window!", NULL, NULL);

	glfwMakeContextCurrent(win);

	gladLoadGL();


	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	//Shader outliningProgram("outlining.vert", "outlining.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);


	Camera cam(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	Model statue("models/crow/scene.gltf");

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	while (!glfwWindowShouldClose(win))
	{
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "YoutubeOpenGL - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(win, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.Inputs(win);
		cam.updateMatrix(45.0f, 0.1f, 100.0f);


		statue.Draw(shaderProgram, cam);


		glfwSwapBuffers(win);

		glfwPollEvents();
	}

	shaderProgram.Delete();


	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
 }