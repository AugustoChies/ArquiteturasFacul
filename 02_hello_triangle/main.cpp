/*
"Hello Triangle". Just the basics + shaders + 2 buffers
Dependencies:
GLEW and GLFW3 to start GL.
Based on OpenGL 4 Example Code.
*/
#include <iostream>
#include "gl_utils.h"		// utility stuff discussed in previous tutorials is here
#include <GL/glew.h>		// include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include"stb_image.h"

#define GL_LOG_FILE "gl.log"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 600;
int g_gl_height = 400;
GLFWwindow *g_window = NULL;

glm::mat4 trans(1.f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 camerafront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 cameraDirection = glm::normalize(cameraPos - (cameraPos + camerafront));
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

glm::mat4 view(1.f);


float fov = 60.f;
float nearPlane = 0.1f;
float farPlane = 1000.f;
glm::mat4 projectionMatrix(1.f);


glm::vec3 lightPos(3.0f, 3.0f, 3.0f);

glm::mat4 lmodel = glm::mat4();

float lastX = 0;
float lastY = 0;
float pitch = 0;
float yaw = -90;
bool firstMouse = true;

bool initShader(const char* vsfile, const char* fsfile,GLuint &program)
{
	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str(vsfile, vertex_shader, 1024 * 256);
	parse_file_into_str(fsfile, fragment_shader, 1024 * 256);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	program = glCreateProgram();

	glAttachShader(program, fs);
	glAttachShader(program, vs);
	glLinkProgram(program);


	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			program);
		print_programme_info_log(program);
		return false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.3;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camerafront = glm::normalize(front);
}

void bindFrameBuffer(int frameBuffer, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

void unbindCurrentFrameBuffer() {//call to switch to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_gl_width, g_gl_height);
}



int main() {
	restart_gl_log();
	// all the start-up code for GLFW and GLEW is called here
	start_gl();

	lmodel = glm::translate(lmodel, lightPos);
	lmodel = glm::scale(lmodel, glm::vec3(0.2f));

	static const GLfloat light_vertices[] = {
		-1.0f, -1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f,  
		1.0f,  1.0f, -1.0f,  
		1.0f,  1.0f, -1.0f,  
		-1.0f,  1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 

		-1.0f, -1.0f,  1.0f, 
		1.0f, -1.0f,  1.0f,  
		1.0f,  1.0f,  1.0f,  
		1.0f,  1.0f,  1.0f,  
		-1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f,  1.0f, 

		-1.0f,  1.0f,  1.0f, 
		-1.0f,  1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f,  1.0f, 
		-1.0f,  1.0f,  1.0f, 

		1.0f,  1.0f,  1.0f,  
		1.0f,  1.0f, -1.0f,  
		1.0f, -1.0f, -1.0f,  
		1.0f, -1.0f, -1.0f,  
		1.0f, -1.0f,  1.0f,  
		1.0f,  1.0f,  1.0f,  

		-1.0f, -1.0f, -1.0f, 
		1.0f, -1.0f, -1.0f,  
		1.0f, -1.0f,  1.0f,  
		1.0f, -1.0f,  1.0f,  
		-1.0f, -1.0f,  1.0f, 
		-1.0f, -1.0f, -1.0f, 

		-1.0f,  1.0f, -1.0f, 
		1.0f,  1.0f, -1.0f,  
		1.0f,  1.0f,  1.0f,  
		1.0f,  1.0f,  1.0f,  
		-1.0f,  1.0f,  1.0f, 
		-1.0f,  1.0f, -1.0f
	};

	static const GLfloat g_vertex_buffer_data[] = {
		//pos                 //normal               //texture
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,	 1.0f,0.0f,		
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,	 1.0f,1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,    0.0f,1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,	 0.0f,1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,	 0.0f,0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,	 1.0f,0.0f,
		

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,    0.0f,1.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,		1.0f,1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,		1.0f,0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,		1.0f,0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,	0.0f,0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,	0.0f,1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,	0.0f,1.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,	1.0f,1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,	1.0f,0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,	1.0f,0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,	0.0f,0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,	0.0f,1.0f,

		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,	1.0f,0.0f,		
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,	1.0f,1.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,	0.0f,1.0f,	
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,	0.0f,1.0f,		
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,	0.0f,0.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,	1.0f,0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,	0.0f,1.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,	1.0f,1.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,	1.0f,0.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,	1.0f,0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,	0.0f,0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,	0.0f,1.0f,

		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,	1.0f,0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,	1.0f,1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,	0.0f,1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,	0.0f,1.0f,		
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,	0.0f,0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,	1.0f,0.0f
		
		
	};

	static const GLfloat screen_vertices[] = {
		-1, -1, -1.0f,				0,0,
		1, 1, -1.0f,		        1.0f,1.0f,
		-1, 1, -1.0f,				0,1.0f,
		

		1,-1, -1.0f,                1.0f,0,
		1,1, -1.0f,					1.0f,1.0f,
		-1,-1, -1.0f,			    0,0
	};

	GLuint points_vbo;
	glGenBuffers( 1, &points_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW );

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("vitas.jpg", &width, &height, &nrChannels, 0);
	if (data != NULL)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	/* create the VAO.
	we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
	the memory should be fetched for vertex shader input variables 0, and 1,
	respectively. we also have to explicitly enable both 'attribute' variables.
	'attribute' is the older name for vertex shader 'in' variables. */
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), NULL);
	//glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );	
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT) , (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	
	
	
	GLuint shader_programme;	
	initShader("test_vs.glsl", "test_fs.glsl", shader_programme );
	
	//LIGHTS
	GLuint light_vbo;
	glGenBuffers(1, &light_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);

	//LIGHT VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);	

	GLuint light_shader_programme;
	initShader("light_vs.glsl", "light_fs.glsl", light_shader_programme);
	
	//CRIA FBO
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	unsigned int colorBuffers[2];
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, g_gl_width, g_gl_height, 0, GL_RGB, GL_FLOAT, NULL
		);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);



	GLuint screen_vbo;
	glGenBuffers(1, &screen_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertices), screen_vertices, GL_STATIC_DRAW);

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint screen_shader_programme;
	initShader("screen_vs.glsl", "screen_fs.glsl", screen_shader_programme);
	

	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, g_gl_width, g_gl_height, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}

	GLuint shaderBlur;
	initShader("screen_vs.glsl", "gaussblur_fs.glsl", shaderBlur);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

	glEnable( GL_CULL_FACE ); // cull face
	glCullFace( GL_FRONT );		// cull back face
	glFrontFace( GL_CW );			// GL_CCW for counter clock-wise
	glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	bool ortho = false;
	float changetimer = 0.0;
	
	//glfwSetKeyCallback(g_window, key_callback);
	glfwSetCursorPosCallback(g_window, mouse_callback);
	while ( !glfwWindowShouldClose( g_window ) ) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		changetimer -= 1 * deltaTime;
		_update_fps_counter( g_window );
		// wipe the drawing surface clear
		glClearColor(0.4f, 0.4f, 0.6f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport( 0, 0, g_gl_width, g_gl_height );

		bindFrameBuffer(fbo, g_gl_width, g_gl_height);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		glEnable(GL_DEPTH_TEST);
		
		glUseProgram(shader_programme);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unsigned int campositionu = glGetUniformLocation(shader_programme, "viewPos");
		glUniform3fv(campositionu, 1, glm::value_ptr(cameraPos));

		unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		cameraDirection = glm::normalize(cameraPos - (cameraPos + camerafront));
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		view = glm::lookAt(cameraPos, cameraPos + camerafront, cameraUp);

		unsigned int viewLoc = glGetUniformLocation(shader_programme, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		if (!ortho)
		{
			projectionMatrix = glm::perspective(
				glm::radians(fov),
				static_cast<float>(g_gl_width) / g_gl_height,
				nearPlane,
				farPlane
			);
		}
		else
		{
			projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
		}

		unsigned int projLoc = glGetUniformLocation(shader_programme, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


		unsigned int lightpositionu = glGetUniformLocation(shader_programme, "lightPos");
		glUniform3fv(lightpositionu, 1, glm::value_ptr(lightPos));

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Draw light
		glUseProgram(light_shader_programme);
		unsigned int ltransformLoc = glGetUniformLocation(light_shader_programme, "model");
		glUniformMatrix4fv(ltransformLoc, 1, GL_FALSE, glm::value_ptr(lmodel));
		unsigned int lviewLoc = glGetUniformLocation(light_shader_programme, "view");
		glUniformMatrix4fv(lviewLoc, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int lprojLoc = glGetUniformLocation(light_shader_programme, "projection");
		glUniformMatrix4fv(lprojLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		unbindCurrentFrameBuffer();

		//BLUR
		bool horizontal = true, first_iteration = true;
		int amount = 100;
		glUseProgram(shaderBlur);
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			unsigned int horiz = glGetUniformLocation(shaderBlur, "horizontal");
			glUniform1i(horiz,(int)horizontal);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]
			);
			glBindVertexArray(screenVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
			unbindCurrentFrameBuffer();
		}
		unbindCurrentFrameBuffer();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(screen_shader_programme);
		glUniform1i(glGetUniformLocation(screen_shader_programme, "scene"), 0);
		glUniform1i(glGetUniformLocation(screen_shader_programme, "bloomBlur"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[1]);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, pingpongBuffer[1]);

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// update other events like input handling
		glfwPollEvents();

		float cameraSpeed = 4.0f * deltaTime;

		if ( GLFW_PRESS == glfwGetKey( g_window, GLFW_KEY_ESCAPE ) ) {
			glfwSetWindowShouldClose( g_window, 1 );
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_W)) {
			cameraPos += cameraSpeed * camerafront;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_A)) {
			cameraPos -= glm::normalize(glm::cross(camerafront, cameraUp)) * cameraSpeed;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_S)) {
			cameraPos -= cameraSpeed * camerafront;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_D)) {
			cameraPos += glm::normalize(glm::cross(camerafront, cameraUp)) * cameraSpeed;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_R)) {
			cameraPos += cameraUp * cameraSpeed;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_F)) {
			cameraPos -= cameraUp * cameraSpeed;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_O)) {
			if (changetimer <= 0)
			{
				ortho = !ortho;
				changetimer = 0.2f;
			}
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_E)) {
			//cameraUp = glm::rotate(cameraUp, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0)); Isso não consegui fazer, terntei usar os senos e cossenos e tambem o glm::rotate
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_Q)) {
			//cameraUp = glm::rotate(cameraUp, glm::radians(-1.0f), glm::vec3(0.0, 0.0, 1.0));
		}

		

		// put the stuff we've been drawing onto the display
		
		glfwSwapBuffers( g_window );
	}
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(2, colorBuffers);
	glDeleteFramebuffers(2, pingpongBuffer);
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
