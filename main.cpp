//include libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//create variables
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;
unsigned int VBO;

//create an array for verticies for the triangle
float triangle_1_vert[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

//create the vertex shader
const char *vertexShaderSource = "#version 450 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"	
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//create the fragment shader
const char *fragentShaderSource = "#version 450 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
		"FragColor = vec4(0.0f, 0.0f, 1.0f, 0.0f);\n"
	"}\n";

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//create a vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//bind vertex array object
	glBindVertexArray(VAO);
	//copy verticies into a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1_vert), triangle_1_vert, GL_STATIC_DRAW);
	//set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//main render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//rendering commands
		//set the window color
		glClearColor(0.00, 1.00, 0.00, 1.00);
		glClear(GL_COLOR_BUFFER_BIT);
		//draw the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
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