#include<iostream>
#include<math.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

#include "SSyst.h"
#include "simInput.h"

#define versize 42416// 8 * (2 + 32 * 15) * 11
#define indsize 23040// 8 * ( 2 * 32 + 14 * 2 * 32) * 3
#define lversize 1446// (2 + 32 * 15) * 3
#define lindsize 2880//( 2 * 32 + 14 * 2 * 32) * 3

// Vertices coordinates
GLfloat vertices[versize];
GLuint indices[indsize];
GLfloat lightVertices[lversize];
GLuint lightIndices[lindsize];

void setupver(DATA_PREC angle, DATA_PREC radius, DATA_PREC diameter, GLfloat* ver, int beg);
void setupplanetver(GLfloat* ver, SSyst* us, simInput* sI);
void setupind(GLuint* ind, int it);
void setuplver(DATA_PREC diameter, GLfloat* ver);

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(winx, winy, "Uklad sloneczny Krzysztof Przeslawski", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	SSyst* us = new SSyst();
	simInput* sI = new simInput(window);

	setupplanetver(vertices,us,sI);
	setuplver(sI->ss * us->sunD(),lightVertices);

	for (int i = 0; i < 8; i++)
		setupind(indices, i);

	setupind(lightIndices, 0);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, winx, winy);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	// Texture
	Texture merq("textures/merc2.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	merq.texUnit(shaderProgram, "tex0", 0);
	Texture ven("textures/ven.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	ven.texUnit(shaderProgram, "tex1", 0);
	Texture eart("textures/eart.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	eart.texUnit(shaderProgram, "tex2", 0);
	Texture mars("textures/mar2.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	mars.texUnit(shaderProgram, "tex3", 0);
	Texture jup("textures/jup.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	jup.texUnit(shaderProgram, "tex4", 0);
	Texture sat("textures/sat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	sat.texUnit(shaderProgram, "tex5", 0);
	Texture ura("textures/ura.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	ura.texUnit(shaderProgram, "tex6", 0);
	Texture nep("textures/nep.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	nep.texUnit(shaderProgram, "tex7", 0);


	// Original code from the tutorial
	/*Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);*/



	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(winx, winy, glm::vec3(0.0f, 2.0f, 0.0f));

	// Main while loop
	VBO* v;
	while (!glfwWindowShouldClose(window))
	{
		sI->timeEvent();

		setupplanetver(vertices, us, sI);

		v = new VBO(vertices, sizeof(vertices));

		VAO1.Bind();
		VAO1.LinkAttrib(*v, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
		
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		// Tells OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		// Binds texture so that is appears in rendering
		// Draw primitives, number of indices, datatype of indices, index of indices

		merq.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(0 * indsize / 8*sizeof(int)));
		ven.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(1 * indsize / 8* sizeof(int)));
		eart.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(2 * indsize / 8* sizeof(int)));
		mars.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(3 * indsize / 8* sizeof(int)));
		jup.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(4 * indsize / 8* sizeof(int)));
		sat.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(5 * indsize / 8* sizeof(int)));
		ura.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(6 * indsize / 8* sizeof(int)));
		nep.Bind();
		glDrawElements(GL_TRIANGLES, indsize / 8, GL_UNSIGNED_INT, (void*)(7 * indsize / 8* sizeof(int)));
		
		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, lindsize, GL_UNSIGNED_INT, 0);


		v->Delete();
		v = nullptr;

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	merq.Delete();
	ven.Delete();
	eart.Delete();
	mars.Delete();
	jup.Delete();
	sat.Delete();
	ura.Delete();
	nep.Delete();

	shaderProgram.Delete();

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();

	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


void setupver( DATA_PREC angle, DATA_PREC radius, DATA_PREC diameter, GLfloat* ver, int beg) {
	//first
	ver[beg] = (radius * cos(angle)) ;
	ver[beg + 1] = (radius * sin(angle)) ;
	ver[beg + 2] = diameter * -0.5 ;

	ver[beg + 3] = 1;
	ver[beg + 4] = 1;
	ver[beg + 5] = 1;

	ver[beg + 6] = 0.5;
	ver[beg + 7] = 0;

	ver[beg + 8] = 0;
	ver[beg + 9] = 0;
	ver[beg + 10] = -1.f;

	for(int i = 0; i < 15; i++)
		for (int i2 = 0; i2 < 32; i2++)
		{
			ver[beg + 11 + i* 352 + i2 * 11] = (cos((i - 7.) * 0.03125 * 2 * M_PI) * diameter * 0.5 * sin(i2 * 0.03125 * 2 * M_PI) + radius * cos(angle));
			ver[beg + 12 + i* 352 + i2 * 11] = (cos((i - 7.) * 0.03125 * 2 * M_PI) * diameter * 0.5 * cos(i2 * 0.03125 * 2 * M_PI)  + radius * sin(angle));
			ver[beg + 13 + i* 352 + i2 * 11] = sin((i-7.)* 0.03125 * 2 * M_PI) * diameter * 0.5 ;
								 
			ver[beg + 14 + i* 352 + i2 * 11] = 1;
			ver[beg + 15 + i* 352 + i2 * 11] = 1;
			ver[beg + 16 + i* 352 + i2 * 11] = 1;
							
			ver[beg + 17 + i* 352 + i2 * 11] = 0.03225 *i2;
			ver[beg + 18 + i* 352 + i2 * 11] = 0.05882 *(i+1.);
								
			ver[beg + 19 + i* 352 + i2 * 11] = sin(i2 * 0.03125 * 2 * M_PI);
			ver[beg + 20 + i* 352 + i2 * 11] = cos(i2 * 0.03125 * 2 * M_PI);
			ver[beg + 21 + i* 352 + i2 * 11] = sin((i - 7.) * 0.03125 * 2 * M_PI);
		}

	//last
	ver[beg + 5291] = (radius * cos(angle)) ;
	ver[beg + 5292] = (radius * sin(angle)) ;
	ver[beg + 5293] = diameter * 0.5 ;

	ver[beg + 5294] = 1;
	ver[beg + 5295] = 1;
	ver[beg + 5296] = 1;

	ver[beg + 5297] = 0.5;
	ver[beg + 5298] = 1;

	ver[beg + 5299] = 0;
	ver[beg + 5300] = 0;
	ver[beg + 5301] = 1.f;

}

void setuplver(DATA_PREC diameter, GLfloat* ver) {
	//first
	ver[0] = 0;
	ver[1] = 0;
	ver[2] = diameter * -0.5;

	for (int i = 0; i < 15; i++)
		for (int i2 = 0; i2 < 32; i2++)
		{
			ver[3 + i2 * 3 + i * 96] = cos((i - 7.) * 0.03125 * 2 * M_PI) * diameter * 0.5 * sin(i2 * 0.03125 * 2 * M_PI);
			ver[4 + i2 * 3 + i * 96] = cos((i - 7.) * 0.03125 * 2 * M_PI) * diameter * 0.5 * cos(i2 * 0.03125 * 2 * M_PI);
			ver[5 + i2 * 3 + i * 96] = sin((i - 7.) * 0.03125 * 2 * M_PI) * diameter * 0.5;

		}

	//last
	ver[3845] = 0;
	ver[3846] = 0;
	ver[3847] = diameter * 0.5;

}

void setupplanetver(GLfloat* ver, SSyst* us, simInput* sI) {
	
	int vi = 5302;
	setupver(us->positionvector(sI->time, us->merqR()), us->merqR(), sI->ps * us->merqD(), ver,0*vi); 
	setupver(us->positionvector(sI->time, us->venR()), us->venR(), sI->ps * us->venD(), ver, 1*vi);
	setupver(us->positionvector(sI->time, us->eartR()), us->eartR(), sI->ps * us->eartD(), ver,2*vi); 
	setupver(us->positionvector(sI->time, us->marR()), us->marR(), sI->ps * us->marD(), ver,3*vi);
	setupver(us->positionvector(sI->time, us->jupR()),us->jupR(), sI->ps * us->jupD(), ver,4*vi);
	setupver(us->positionvector(sI->time, us->satR()), us->satR(), sI->ps * us->satD(), ver,5*vi);
	setupver(us->positionvector(sI->time, us->uraR()), us->uraR(), sI->ps * us->uraD(), ver,6*vi);
	setupver(us->positionvector(sI->time, us->neptR()), us->neptR(), sI->ps * us->neptD(), ver,7*vi); 
	
}

void setupind(GLuint* ind, int it) {
	int begv = it * 482;
	int begi = it * 2880;
	for (int i = 0; i < 32; i++)
	{
		ind[begi + i * 3    ] = begv;
		ind[begi + i * 3 + 1] = begv + i + 1;
		ind[begi + i * 3 + 2] = begv + i + 2;
		if(i == 31)
			ind[begi + i * 3 + 2] = begv + 1;

	}

	for (int i = 0; i < 14; i++) {
		for (int i2 = 0; i2 < 32; i2++) {
			ind[begi + i * 192 + i2 * 6 + 96] = begv + i * 32 + i2 + 1;
			ind[begi + i * 192 + i2 * 6 + 97] = begv + i * 32 + i2 + 2;
			ind[begi + i * 192 + i2 * 6 + 98] = begv + i * 32 + i2 + 33;

			ind[begi + i * 192 + i2 * 6 + 99] = begv + i * 32 + i2 + 33;
			ind[begi + i * 192 + i2 * 6 + 100] = begv + i * 32 + i2 + 34;
			ind[begi + i * 192 + i2 * 6 + 101] = begv + i * 32 + i2 + 2;

			if (i2 == 31) {
				ind[begi + i * 192 + i2 * 6 + 97] = begv + i * 32 + 1;

				ind[begi + i * 192 + i2 * 6 + 100] = begv + i * 32 + 33;
				ind[begi + i * 192 + i2 * 6 + 101] = begv + i * 32 + 1;
			}

		}
	}

	for (int i = 0; i < 32; i++)
	{
		ind[begi + i * 3 + 2784] = begv + 481;
		ind[begi + i * 3 + 2785] = begv + i + 449;
		ind[begi + i * 3 + 2786] = begv + i + 450;
		if (i == 31)
			ind[begi + i * 3 + 2786] = begv + 449;

	}
}