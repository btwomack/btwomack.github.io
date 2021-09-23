

/* Header Inclusions */
#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

// GLM Math Header Inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL image loader inclusion
#include "SOIL2/SOIL2.h"

using namespace std; // Standard namespace

//#define WINDOW_TITLE "Modern OpenGL" // Window title Macro

/* Shader program Macro */
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

/* Variable declarations for shader, window size initialization, buffer and array objects */
GLint shaderProgram, lampShaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, lightVAO, EBO, texture;
GLfloat degrees = glm::radians(-45.0f);

GLfloat cameraSpeed = 0.0005F; // Movement speed per frame

GLchar currentKey; // Will store key pressed

GLfloat lastMouseX = 400, lastMouseY = 300; // Locks mouse cursor at the center of the screen
GLfloat mouseXOffset, mouseYOffset, yaw = 0.0f, pitch = 0.0f; // Mouse offset, yaw, and pitch variables
GLfloat sensitivity = 0.005f; // Used for mouse / camera rotation sensitivity

bool mouseDetected = true; // Initially true when mouse movement is detected


// Subject position and scale
glm::vec3 bottlePosition(0.0f, 0.0f, 0.0f);
glm::vec3 bottleScale(2.0f);

// Object and light color
glm::vec3 objectColor(0.6f, 0.5f, 0.75f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// Light position and scale
glm::vec3 lightPosition(0.5f, 0.5f, -3.0f);
glm::vec3 lightScale(0.3f);

// Global vector declarations
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Initial camera position. Placed 5 units in Z
glm::vec3 CameraUpY = glm::vec3(0.0f, 1.0f, 0.0f); // Temporary y unit vector
glm::vec3 CameraForwardZ = glm::vec3(0.0f, 0.0f, -1.0f); // Temporary z unit vector
glm::vec3 front; // Temporary z unit vector for mouse


/* Function Prototypes */
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);
void UKeyboard(unsigned char key, int s, int y);
void UKeyReleased(unsigned char key, int x, int y);
void UMouseClick(int button, int state, int x, int y);
void UMouseMove(int x, int y);
void UMousePressedMove(int x, int y);



/* Vertex Shader Source Code */
const GLchar * vertexShaderSource = GLSL(330,

	layout (location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
	layout (location = 1) in vec3 normal; // Color data from Vertex Attrib Pointer 1
	layout (location = 2) in vec2 textureCoordinate;

	out vec3 Normal; // For outgoing normals to fragment shader
	out vec3 FragmentPos; // For Outgoing color / pixels to fragment shader
	out vec2 mobileTextureCoordinate;

	// Global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

void main() {

	gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
	FragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only
	Normal = mat3(transpose(inverse(model))) * normal; // Get normal vectors in world space only

	}
);

/* Fragment Shader Source Code */
const GLchar * fragmentShaderSource = GLSL(330,

		in vec2 mobileTextureCoordinate;

		in vec3 Normal; // For incoming normals
		in vec3 FragmentPos; // For incoming fragmnet position

		out vec4 gpuTexture; // Variable to pass color data to the GPU

		uniform sampler2D uTexture; // Useful when working with multiple textures

		uniform vec3 objectColor; // Uniform / Global variables for object color, light color. light position, and camera/view position

		uniform vec3 lightcolor;
		unifomr vec3 lightPos;
		uniform vec3 viewPosition;

	void main() {

		/* Phong lighting model calculations to generate ambient, diffuse, and specular components */

		// Calculate ambient lighting
		float ambientStrength = 0.1f; // Set ambient or global lighting strength
		vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

		// Calculate diffuse lighting
		vec3 norm = normalize(Normal); // Normalize vectors to 1 unit
		vec3 lightDirection = normalize(lightPos- FragmentPos); // Calculate distance (light direction) between light source and fragment/pixels
		float impact = max(dot(norm, lightDirection), 0.0f); // Calculate diffuse impact by generating dot product of normal and light
		vec3 diffuse = impact * lightColor; // Generate diffuse light color

		// Calculate specular lighting
		float specularIntensity = 0.8f; // Set specular light strength
		float highlightSize = 16.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - FragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(-lightDirection, norm); // Calculate reflection vector

		// Calculate specular component
		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0f), highlightSize);
		vec3 specular = specularIntensity * specularComponent * lightColor;

		// Calculate phong result
		vec3 phong = (ambient + diffuse + specular) * objectColor;

		gpuTexture = vec4(phong, 1.0f); // send lighting results to GPU
		gpuTexture = texture(uTexture, mobileTextureCoordinate); // sends color data to the GPU for rendering
	}
);

/* Lamp Shader Source Code */
const GLchar * lampVertexShaderSource = GLSL(330,

		layout (location = 0) in vec3 position;

		// Uniform / Global variables for the transform matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main(){

			gl_Position = projection * view * model * vec4(position, 1.0f); // Transform vertices into clip coordinates
		}
);

/* Fragment Shader Source Code */
const GLchar * lampFragmentShaderSource = GLSL(330,

		out vec4 color; // For outgoing lamp color to the GPU

		void main(){

			color = vec4(1.0f); // Set color to white

		}
);



/* Main Program */
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


	// Use the Shader program
	glUseProgram(shaderProgram);
	glClearColor(0.0f,  0.0f, 0.0f, 1.0f); // Sets background color
	glutDisplayFunc(URenderGraphics);

	glutKeyboardFunc(UKeyboard); // Detects key press
	glutKeyboardUpFunc(UKeyReleased);

	glutMouseFunc(UMouseClick); // Detects Mouse clicks
	glutPassiveMotionFunc(UMouseMove); // Detects mouse movement
	glutMotionFunc(UMousePressedMove); // Detects Mouse press and movement

	glutMainLoop();


	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}

/* Resize the window */
void UResizeWindow(int w, int h) {
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

/* Renders Graphics */
void URenderGraphics(void) {
	glEnable(GL_DEPTH_TEST); // Enable z-depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

	GLint modelLoc, viewLoc, projLoc, objectColorLoc, lightColorLoc, lightPositionLoc, viewPositionLoc;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glBindVertexArray(VAO); // Activate the Vertex Array Object before rendering and transforming them

	// Camera Movement logic
	if(currentKey == 'w')
		cameraPosition += cameraSpeed * CameraForwardZ;

	if(currentKey == 's')
		cameraPosition -= cameraSpeed * CameraForwardZ;

	if(currentKey == 'a')
		cameraPosition -= glm::normalize(glm::cross(CameraForwardZ, CameraUpY)) * cameraSpeed;

	if(currentKey == 'd')
		cameraPosition += glm::normalize(glm::cross(CameraForwardZ, CameraUpY)) * cameraSpeed;

	CameraForwardZ = front; // Replaces camera forward vector with  Radians normalized as a unit vector


	// Transforms the object
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Place the object at the center of the viewport
	model = glm::rotate(model, degrees, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate the object 45 degrees on the X
	model = glm::scale(model,glm::vec3(2.0f, 2.0f, 2.0f)); // Increase the object size by a scale of 2

	// Transforms the camera
	glm::mat4 view;
	view = glm::lookAt(CameraForwardZ, cameraPosition, CameraUpY);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	// Creates a Perspective Projection
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform Matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Reference matrix uniforms from the shader program for the color, light color, light position, and camera position
	objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
	viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

	// Pass color, light, and camera data to the shader program's corresponding uniforms
	glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	/* Use the lamp shader and activate the lamp vertex array object for rendering and transforming */
	glUseProgram(lampShaderProgram);
	glBindVertexArray(lightVAO);

	// Transform the smaller cube used as a visual que for the light source
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, lightScale);

	// Reference matrix uniforms from the lamp shader program
	modelLoc = glGetUniformLocation(lampShaderProgram, "model");
	viewLoc = glGetUniformLocation(lampShaderProgram, "view");
	projLoc = glGetUniformLocation(lampShaderProgram, "projection");

	// Pass matrix data to the lamp shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	glBindTexture(GL_TEXTURE_2D, texture);

	// Draws the triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); // Deactivate the Vertex Array Object
	glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. Similar to GL flush

}

/* Implements the UMouseClick function */
void UMouseClick(int button, int state, int x, int y) {
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		cout << "Left Mouse Button Clicked!" << endl;
	}
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		cout << "Left Mouse Button Released!" << endl;
	}
}

/* Implements the UMovePressedMove function */
void UMousePressedMove(int x, int y) {
	cout << "Mouse pressed and moved!" << endl;
}
/* Creates the Shader Program */
void UCreateShader() {

		// Vertex Shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates the Vertex shader
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attaches the vertex shader to the source code
		glCompileShader(vertexShader); // Compiles the vertex shader

		// Fragment Shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates the Fragment Shader
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attaches the fragment shader to the source code
		glCompileShader(fragmentShader); // Compiles fragment shader

		// Shader Program
		shaderProgram = glCreateProgram(); // Creates the shader program and returns an id
		glAttachShader(shaderProgram, vertexShader); // Attach Vertex shader to the shader program
		glAttachShader(shaderProgram, fragmentShader); // Attach fragment shader to the shader program
		glLinkProgram(shaderProgram); // Links vertex and fragment shaders to shader program

		// Delete the vertex and fragment shaders once linked
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Lamp vertex shader
		GLint lampVertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates the Vertex Shader
		glShaderSource(lampVertexShader, 1, &lampVertexShaderSource, NULL); // Attaches the Vertex shader to the source code
		glCompileShader(lampVertexShader); // Compiles the Vertex shader

		// Lamp fragment shader
		GLint lampFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates fragment shader
		glShaderSource(lampFragmentShader, 1, &lampFragmentShaderSource, NULL); // Attaches fragment shader to shader program
		glCompileShader(lampFragmentShader); // Compiles the fragment shader

		// Lamp shader program
		lampShaderProgram = glCreateProgram(); // Creates the shader program and returns an id
		glAttachShader(lampShaderProgram, lampVertexShader); //  Attach the vertex shader to the shader program
		glAttachShader(lampShaderProgram, lampFragmentShader); // Attach the fragment shader to the shader program
		glLinkProgram(lampShaderProgram);

		// Delete the lamp shaders once linked
		glDeleteShader(lampVertexShader);
		glDeleteShader(lampFragmentShader);

}


/* Creates Buffer and Array Objects */
void UCreateBuffers() {
	GLfloat vertices[] = {

										// Positions 		 // Normals          // Textures

										-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
										 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
										 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
										 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
										-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
										-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,


										-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
										 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
										 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
										 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
										-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
										-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0f,

										-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
										-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
										-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
										-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
										-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
										-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

										 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
										 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
										 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
										 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
										 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
										 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

									    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
										 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
										 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
										 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
									    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
									    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

										-0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
									     0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
										 0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,  1.0f, 0.0f,
										 0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,  1.0f, 0.0f,
										-0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
										-0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f, 1.0f

									};

	// Generate buffer ids
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Activate the Vertex Array Object before binding and setting and VBO's and Vertex Attribute Pointers
	glBindVertexArray(VAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy the vertices to VBO

	// Set attribute pointer to 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // Enable vertex attribute

	// Set attribute pointer 1 to hold Normal data
	glVertexAttribPointer(1,, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

 	// Set attribute pointer to 2 to hold texture data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 *sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // Enable vertex attribute

	glBindVertexArray(0); // Deactivates the VAO which is a good practice

	// Generate buffer ids for lamp
	glGenVertexArrays(1, &lightVAO); // Vertex Array Object for vertex copies to serve as light source

	// Activate the Vertex Array object before binding and setting any VBO's and Vertex Attribute Pointers
	glBindVertexArray(lightVAO);

	// Referencing the same VBO for its verticies
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Set attribute pointer 0 to hold position data for lamp
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

/* Generate and load the texture */
void UGenerateTexture(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("waterfall.jpg", &width, &height, 0, SOIL_LOAD_RGB); // Load texture file

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

}

/* Implements the UKeyboard function */
void UKeyboard(unsigned char key, GLint x, GLint y) {
	switch(key) {
		case 'w':
					currentKey = key;
					cout << "You pressed W!" << endl;
					break;

		case 's':
					currentKey = key;
					cout << "You pressed S!" << endl;
					break;

		case 'a':
					currentKey = key;
					cout << "You pressed A!" << endl;
					break;

		case 'd':
					currentKey = key;
					cout << "You pressed D!" << endl;
					break;

		default:
					cout << "Press a key!" << endl;
	}
}

/* Implements the UKeyReleased function */
void UKeyReleased(unsigned char key, GLint x, GLint y) {

					cout << "Key released!" << endl;
					currentKey = '0';
}

/* Implements the UMouseMove */
void UMouseMove(int x, int y) {

	// Immediately replaces center locked coordinates with new mouse coordinates
	if(mouseDetected) {

			lastMouseX = x;
			lastMouseY = y;
			mouseDetected = false;
		}

	// Gets the direction the mouse was moved in x and y
	mouseXOffset = x - lastMouseX;
	mouseYOffset = lastMouseY - y; // Inverted y axis

	// Updates with new mouse coordinates
	lastMouseX = x;
	lastMouseY = y;

	// Applies sensitivity to mouse direction
	mouseXOffset *= sensitivity;
	mouseYOffset *= sensitivity;

	// Accumulates the yaw and pitch variables
	yaw += mouseXOffset;
	pitch += mouseYOffset;

	// Orbits around the center
	front.x = 10.0f * cos(yaw);
	front.y = 10.0f * sin(pitch);
	front.z = sin(yaw) * cos(pitch) * 10.0f;

}
