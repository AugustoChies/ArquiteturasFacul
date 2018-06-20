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
int g_gl_width = 640;
int g_gl_height = 480;
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


glm::vec3 lightPos(1.0f, 1.0f, 3.0f);

glm::mat4 lmodel = glm::mat4();




int main() {
	restart_gl_log();
	// all the start-up code for GLFW and GLEW is called here
	start_gl();

	lmodel = glm::translate(lmodel, lightPos);
	lmodel = glm::scale(lmodel, glm::vec3(0.2f));

	/* OTHER STUFF GOES HERE NEXT */
	//GLfloat points[] = { 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,
	//					0.0f, -1.0f, 0.0f};

	//GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	//					  1.0f, 0.0f, 0.0f};

	//GLfloat vertices[] = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	//					  -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f };


	static const GLfloat light_vertices[] = {
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f
	};

	static const GLfloat g_vertex_buffer_data[] = {
		//position                     //color                      //normal          //uv
		-1.0f,-1.0f,-1.0f,            0.583f,  0.771f,  0.014f,     0.0f,0.0f,-1.0f,   1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,			  0.609f,  0.115f,  0.436f,     0.0f,0.0f,1.0f,    0.0f,1.0f,
		-1.0f, 1.0f, 1.0f,			  0.327f,  0.483f,  0.844f,     0.0f,0.0f,1.0f,    0.0f,0.0f,
		1.0f, 1.0f,-1.0f, 			  0.822f,  0.569f,  0.201f,     0.0f,0.0f,-1.0f,   0.0f,0.0f,
		-1.0f, 1.0f,-1.0f,			  0.310f,  0.747f,  0.185f,     0.0f,0.0f,-1.0f,   1.0f,0.0f,
		1.0f,-1.0f, 1.0f,			  0.597f,  0.770f,  0.761f,     0.0f,0.0f,1.0f,	   1.0f,1.0f,
		1.0f,-1.0f,-1.0f,			  0.359f,  0.583f,  0.152f,     0.0f,0.0f,-1.0f,   0.0f,1.0f,
		1.0f, 1.0f, 1.0f,			  0.279f,  0.317f,  0.505f,     0.0f,0.0f,1.0f,	   1.0f,0.0f
	};



	GLuint indices[] = {  
		0,1,2,3,0,4,5,0,6,3,6,0,0,2,4,5,1,0,2,1,5,7,6,3,6,7,5,7,3,4,7,4,2,7,2,5
	};	

	GLuint points_vbo;
	glGenBuffers( 1, &points_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW );

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//LIGHTS
	GLuint light_vbo;
	glGenBuffers(1, &light_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);


	/* create the VAO.
	we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
	the memory should be fetched for vertex shader input variables 0, and 1,
	respectively. we also have to explicitly enable both 'attribute' variables.
	'attribute' is the older name for vertex shader 'in' variables. */
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), NULL);
	//glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );	
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT) , (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void*)(9 * sizeof(GL_FLOAT)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);


	//LIGHT VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);



	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str( "test_vs.glsl", vertex_shader, 1024 * 256 );
	parse_file_into_str( "test_fs.glsl", fragment_shader, 1024 * 256 );

	GLuint vs = glCreateShader( GL_VERTEX_SHADER );
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource( vs, 1, &p, NULL );
	glCompileShader( vs );

	// check for compile errors
	int params = -1;
	glGetShaderiv( vs, GL_COMPILE_STATUS, &params );
	if ( GL_TRUE != params ) {
		fprintf( stderr, "ERROR: GL shader index %i did not compile\n", vs );
		print_shader_info_log( vs );
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
	p = (const GLchar *)fragment_shader;
	glShaderSource( fs, 1, &p, NULL );
	glCompileShader( fs );

	// check for compile errors
	glGetShaderiv( fs, GL_COMPILE_STATUS, &params );
	if ( GL_TRUE != params ) {
		fprintf( stderr, "ERROR: GL shader index %i did not compile\n", fs );
		print_shader_info_log( fs );
		return 1; // or exit or something
	}
	
	GLuint shader_programme = glCreateProgram();

	glAttachShader( shader_programme, fs );
	glAttachShader( shader_programme, vs );
	glLinkProgram( shader_programme );



	glGetProgramiv( shader_programme, GL_LINK_STATUS, &params );
	if ( GL_TRUE != params ) {
		fprintf( stderr, "ERROR: could not link shader programme GL index %i\n",
						 shader_programme );
		print_programme_info_log( shader_programme );
		return false;
	}

	//LIGHTSHADER
	/*char vertex_shaderl[1024 * 256];
	char fragment_shaderl[1024 * 256];
	parse_file_into_str("light_vs.glsl", vertex_shaderl, 1024 * 256);
	parse_file_into_str("light_fs.glsl", fragment_shaderl, 1024 * 256);

	GLuint lvs = glCreateShader(GL_VERTEX_SHADER);
	p = (const GLchar *)vertex_shaderl;
	glShaderSource(lvs, 1, &p, NULL);
	glCompileShader(lvs);

	// check for compile errors
	params = -1;
	glGetShaderiv(lvs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", lvs);
		print_shader_info_log(lvs);
		return 1; // or exit or something
	}

	GLuint lfs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shaderl;
	glShaderSource(lfs, 1, &p, NULL);
	glCompileShader(lfs);

	// check for compile errors
	glGetShaderiv(lfs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", lfs);
		print_shader_info_log(lfs);
		return 1; // or exit or something
	}

	GLuint light_shader_programme = glCreateProgram();

	glAttachShader(light_shader_programme, lfs);
	glAttachShader(light_shader_programme, lvs);
	glLinkProgram(light_shader_programme);
	


	glGetProgramiv(light_shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			light_shader_programme);
		print_programme_info_log(light_shader_programme);
		return false;
	}
	*/

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

	glEnable( GL_CULL_FACE ); // cull face
	glCullFace( GL_FRONT );		// cull back face
	glFrontFace( GL_CW );			// GL_CCW for counter clock-wise


	unsigned int lightpositionu = glGetUniformLocation(shader_programme, "lightPos");
	glUniform3fv(lightpositionu , 1, glm::value_ptr(lightPos));
	
	
	std::cout << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z;
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	bool ortho = false;
	float changetimer = 0.0;
	
	//glfwSetKeyCallback(g_window, key_callback);

	while ( !glfwWindowShouldClose( g_window ) ) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		changetimer -= 1 * deltaTime;
		_update_fps_counter( g_window );
		// wipe the drawing surface clear
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport( 0, 0, g_gl_width, g_gl_height );

		unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		cameraDirection = glm::normalize(cameraPos - (cameraPos + camerafront));
		//cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		//cameraUp = glm::cross(cameraDirection, cameraRight);
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
			projectionMatrix = glm::ortho(-5.0f, 5.0f,-5.0f, 5.0f, 0.1f, 100.0f);
		}

		unsigned int projLoc = glGetUniformLocation(shader_programme, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glUseProgram( shader_programme );
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glBindVertexArray( vao );
		
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Draw light
		/*glUseProgram(light_shader_programme);
		unsigned int ltransformLoc = glGetUniformLocation(light_shader_programme, "model");
		glUniformMatrix4fv(ltransformLoc, 1, GL_FALSE, glm::value_ptr(lmodel));
		unsigned int lviewLoc = glGetUniformLocation(light_shader_programme, "view");
		glUniformMatrix4fv(lviewLoc, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int lprojLoc = glGetUniformLocation(light_shader_programme, "projection");
		glUniformMatrix4fv(lprojLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/
	

		// update other events like input handling
		glfwPollEvents();

		float cameraSpeed = 2.0f * deltaTime;

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

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
