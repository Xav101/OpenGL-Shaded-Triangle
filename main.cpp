//include libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//create variables
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;
unsigned int VBO;

//create an array of verticies and colors for the triangle
float triangle_1_vert[] = {
	//verticies         //colors
	-0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	 0.5f, 0.0f, 0.0f,   0.9961f, 0.0039f, 0.9961f,
	 0.0f, 0.5f, 0.0f,   0.0039, 0.9961f, 1.0f
};

//create an array for indicies
//unsigned int indicies[] = {
//	0, 1, 3, //first triangle
//	2, 3, 1 //second triangle
//};

//create the vertex shader
const char *vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 triangleColor;\n"
"void main() {\n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"triangleColor = aColor;\n"
"}";

//create the fragment shader
const char *fragentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"in vec3 triangleColor;\n"
"void main() {\n"
	"FragColor = vec4(triangleColor, 1.0);\n"
"}";

//function to resize the viewport if the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//if the escape key is pressed, specify that the window should close
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//main loop
int main() {
	//initialize glfw and configure the window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLTest", NULL, NULL);
	//return an error and terminate glfw if window creation fails
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//update the context for the window on the current thread
	glfwMakeContextCurrent(window);
	//callback to resize the viewport if the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//if glad fails to initialize throw an error
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;

	//create a vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1_vert), triangle_1_vert, GL_STATIC_DRAW);

	//build and compile the vertex shader
	//create a shader object, referenced by an ID
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach glsl vertex shader code to shader object and compile
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//check for compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//if compilation failed, print errors
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//build and compile the fragment shader
	//create a shader object, referenced by an ID
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//attach glsl fragment shader code to shader object and compile
	glShaderSource(fragmentShader, 1, &fragentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//create the shader program and attach the shaders to it
	//create shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//attach the shaders to the program object and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check for failure
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//delete the shader programs to free up some space
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//create a vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//bind vertex array object
	glBindVertexArray(VAO);
	//copy verticies into a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1_vert), triangle_1_vert, GL_STATIC_DRAW);
	//set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//set color attribute pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//create an element buffer object
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//copy elements into a buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies),	 indicies, GL_STATIC_DRAW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//main render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//rendering commands
		//set the window color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//run the shader program
		glUseProgram(shaderProgram);
		//update the triangle color
		float newColor[] = {
			0.9451f, 0.9569f, 0.0078f
		};

		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float), 3 * sizeof(float), newColor);

		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "triangleColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenColor, 0.0f, 1.0f);

		//draw the triangle
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap the framebuffers to ensure smooth drawing
		glfwSwapBuffers(window);
		//check for events and execute corresponding functions
		glfwPollEvents();
	}
	//if the window should close, terminate glfw and end the program
	glfwTerminate();
	return 0;
}