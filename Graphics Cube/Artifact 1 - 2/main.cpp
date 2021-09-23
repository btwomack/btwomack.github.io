/* Brandon Womack*/

/*Header Inclusions*/
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "SOIL2/SOIL2.h"

/*GLM Math Header Inclusions*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std; //Using standard namespace
#define WINDOW_TITLE "Practice Activity 8" //Window title Macro
//Shader program Macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

/*Variable declarations for the shader, window size
* initialization, and buffer and array objects
*/

GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, texture;
GLfloat degrees = glm::radians(0.0f); //Converts float to degrees

//Cube and light color
glm::vec3 lightColor(0.0f, 1.0f, 0.0f);
glm::vec3 secondLightColor(1.0f, 1.0f, 1.0f);

//Light position and scale
glm::vec3 lightPosition(1.0f, 0.5f, -3.0f);
glm::vec3 lightScale(0.3f);

//ambient specular highlight
glm::vec3 lightStrength(0.1f, 1.0f, 0.5f);

//Camera position
glm::vec3 cameraPosition(-5.0f, 0.0f, -5.0f);

//Camera rotation
float cameraRotation = glm::radians(-45.0f);

//Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);

//Vertex Shader Source Code
const GLchar* vertexShaderSource = GLSL(330,
	layout(location = 0) in vec3 position; //Vertex data from VAP 0
	layout(location = 2) in vec2 textureCoordinate;

	out vec2 mobileTextureCoordinate;
	//Global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main() {
		gl_Position = projection * view * model * vec4(position, 1.0f);

		//Transforms vertices to clip coordinates
		mobileTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y); //Flips the texture horizontal
	}
	);

//Fragment Shader Source Code
const GLchar* fragmentShaderSource = GLSL(330,
	in vec2 mobileTextureCoordinate;
out vec4 gpuTexture; //Variable to pass color data to the GPU
uniform sampler2D uTexture; //Useful when working with multiple textures
void main() {
	gpuTexture = texture(uTexture, mobileTextureCoordinate);
}
);

//Light Vertex Shader Source Code
const GLchar* lightVertexShaderSource = GLSL(330,
	layout(location = 0) in vec3 position; //VAP position 0 for vertex position data
	layout(location = 1) in vec3 normal; //VAP position 1 for normals
	layout(location = 2) in vec2 textureCoordinate; //VAP position 2 for texture coordinate

	out vec3 Normal; //Outoging normals to fragment shader
	out vec3 FragmentPos; //Outgoing color/pixels to fragment shader
	out vec2 mobileTextureCoordinate; //UV coordinates for texture

//Uniform/global variables for the transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f); //Transforms vertices into clip coordinates
	Normal = mat3(transpose(inverse(model))) * normal; //Gets normal vectors in world space onlyand exclude normal translation properties
		FragmentPos = vec3(model * vec4(position, 1.0f)); //Gets fragment/pixel position in world space only(excluding view and projection)
		mobileTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y); //Flips the texture horizontal
}
);

//Light Fragment Shader Source Code
const GLchar* lightFragmentShaderSource = GLSL(330,

	in vec3 Normal; //For incoming normals
	in vec3 FragmentPos; //For incoming fragment posiiton
	in vec2 mobileTextureCoordinate;

	out vec4 result; //For outgoing light color to the GPU

//Uniform/Global variables for object color light color, light position and camera/view position
	uniform vec3 lightColor;
	uniform vec3 secondLightColor;
	uniform vec3 lightPos;
	uniform vec3 viewPosition;
	uniform vec3 lightStrength;
	uniform sampler2D uTexture; //Useful when working with multiple textures

void main() {
	vec3 norm = normalize(Normal); //Normalizes vectors to 1 unit
	vec3 ambient = lightStrength.x * lightColor; //Generates ambient light color
	vec3 ambientTwo = lightStrength.x * secondLightColor;//Generates second ambient light color
	vec3 lightDirection = normalize(lightPos - FragmentPos);

	//Calculates distance (light direction) between light source and fragments / pixels on
	float impact = max(dot(norm, lightDirection), 0.0); //Calculates diffuse impact by generating dot product of normaland light
	vec3 diffuse = impact * lightColor; //Generates diffuse light color
	vec3 viewDir = normalize(viewPosition - FragmentPos); //Calculates view direction
	vec3 reflectDir = reflect(-lightDirection, norm); //Calculates reflection vector

	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), lightStrength.z);
	vec3 specular = lightStrength.y * specularComponent * lightColor;

	//Calculate phong result
	vec3 phongOne = (ambient + diffuse + specular) *
	vec3(texture(uTexture, mobileTextureCoordinate));

	//Hardcode second light position
	lightDirection = normalize(vec3(6.0f, 0.0f, -3.0f) - FragmentPos);
	impact = max(dot(norm, lightDirection), 0.0); //Calculates diffuse impact by generating dot product of normaland light
	diffuse = impact * secondLightColor; //Generates diffuse light color
	viewDir = normalize(viewPosition - FragmentPos); //Calculates view direction
	reflectDir = reflect(-lightDirection, norm); //Calculate reflection vector

	specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), lightStrength.z);

	//Hardcode second light spec
	vec3 specularTwo = 0.1f * specularComponent * secondLightColor;
	vec3 phongTwo = (ambientTwo + diffuse + specularTwo) * vec3(texture(uTexture, mobileTextureCoordinate));

	result = vec4(phongOne + phongTwo, 1.0f); //Sends lighting results
	to GPU
}
);

/*Main Program*/
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);
	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	UCreateShader();
	UCreateBuffers();
	UGenerateTexture();
	//Use the Shader Program
	glUseProgram(shaderProgram);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Sets the background color to black
	glutDisplayFunc(URenderGraphics);
	glutMainLoop();
	//Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	return 0;
}

/*Resizes the window*/
void UResizeWindow(int w, int h) {
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}
/*Renders graphics to the screen*/
void URenderGraphics(void) {
	glEnable(GL_DEPTH_TEST); //Enables z-depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the screen
	glBindVertexArray(VAO); //Activates the vertex array object before rendering and transforming them

	//Transforms the object
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); //Increases the object size by a scale of 2
	model = glm::rotate(model, degrees, glm::vec3(0.0, 1.0f, 0.0f)); //Rotates the object y - 45 degrees
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); //Places the object at the center of the viewport

	//Transforms the camera
	glm::mat4 view;
	view = glm::rotate(view, cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, cameraPosition);

	//Creates a perspective projection
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth /	(GLfloat)WindowHeight, 0.1f, 100.0f);

	//Retrieves and passes transform matrices to the shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	GLint secondLightColorLoc, lightColorLoc, lightPositionLoc, lightStrengthLoc, viewPositionLoc;

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
	lightStrengthLoc = glGetUniformLocation(shaderProgram, "lightStrength");
	secondLightColorLoc = glGetUniformLocation(shaderProgram, "secondLightColor");
	viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

	//Passes color, light, and camera data to the cube shader programs corresponding uniforms
	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(secondLightColorLoc, secondLightColor.r, secondLightColor.g, secondLightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(lightStrengthLoc, lightStrength.x, lightStrength.y, lightStrength.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	glutPostRedisplay();
	glBindTexture(GL_TEXTURE_2D, texture);

	//Draws the triangles
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glBindVertexArray(0); //Deactivates the vertex array object
	glutSwapBuffers(); //Flips the back buffer with the front buffer every frame.
}

/*Creates the Shader Program*/
void UCreateShader() {

	//Vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); //Creates the  vertex shader
	glShaderSource(vertexShader, 1, &lightVertexShaderSource, NULL);

	//Attaches the vertex shader to the source code
	glCompileShader(vertexShader); //Compiles the vertex shader

	//Fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Creates the fragment shader
	glShaderSource(fragmentShader, 1, &lightFragmentShaderSource, NULL);

	//Attaches the fragment shader to the source code
	glCompileShader(fragmentShader); //Compiles the fragment shader

	//Shader Program
	shaderProgram = glCreateProgram(); //Creates the shader program and returns an ID

	glAttachShader(shaderProgram, vertexShader); //Attaches vertex shader to the shader program
	glAttachShader(shaderProgram, fragmentShader); //Attaches fragment shader to the shader program
	glLinkProgram(shaderProgram); //Links vertex and fragment shader to shader progra

	//Deletes the vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

/*Creates the buffer and array objects*/
void UCreateBuffers() {
	GLfloat vertices[] = {
		//Positions //Normals //Texture Coordinates

		//Back Face
		- 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f,

		//Left Face
		0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		//Front Face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,

		//Right Face
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		//Bottom Face
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
	};

	//Generates buffer IDs
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Activates the VAO before binding and setting VBOs and VAPs
	glBindVertexArray(VAO);

	//Activates the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Copies vertices to VBO
	//Sets attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //Enables vertex attribute

	//Sets attribute pointer 1 to hold Normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Sets attribute pointer 2 to hold Texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); //Unbinds the VAO
}

/*Generates and loads the texture*/
void UGenerateTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height;

	unsigned char* image = SOIL_load_image("waterfall.jpg", &width, &height, 0, SOIL_LOAD_RGB);//Loads texture file
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //Unbinds the texture
}
