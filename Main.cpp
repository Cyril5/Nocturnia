#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Shader.h"
#include <cmath>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main()
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	sf::ContextSettings settings;
	settings.depthBits = 24; // nombre de bits par pixel pour le depth buffer (16,24 ou 32)
	settings.stencilBits = 8; // nombre de bits par pixel pour le stencil buffer (0 pour ne pas en créer)
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	//settings.attributeFlags = sf::ContextSettings::Core;
	
	const float FIELD_OF_VIEW = 45.0f; // Camera FOV

	float vertices[] = { // Sommets
		// Vertex					// Texture Coords
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	
	//// EBO : Element Buffer Object
	//unsigned int indices[] = {
	//	0,1,3, // Premier triangle
	//	1,2,3 // Deuxième triangle
	//};


	sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	glewExperimental = GL_TRUE; // Nécessaire dans le profil de base
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Clock clock; // démarre le chrono pour pouvoir obtenir le temps écoulé depuis le début de l'application


	/*
	*---------------------------------------------------------------------------------------------------------------------------------
	*															DRAW OPENGL
	* -------------------------------------------------------------------------------------------------------------------------------
	*/

	glEnable(GL_DEPTH_TEST); // Activer le Z-Buffer


	Shader myShaderProgram("myShader.vsd", "myShader.fsd");

	//VBO : Vertex Buffer Object
	//Ce VBO défini un vertex shader et un fragment shader et dit à OpenGL comment lier les données des sommets aux attributs du vertex shader
	unsigned int VBO;
	glGenBuffers(1, &VBO);


	// VAO : Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//EBO : Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);


	// // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	// Début du VAO qui contiendra des VBO ou EBO
	glBindVertexArray(VAO);
		// 2 .Copier les sommets du triangle dans un tampon pour qu’OpenGL les utilise
		// Début du VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 3. copier le tableau d’indices dans un tampon d’éléments
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Etablir les pointeurs d'attributs des sommets
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		// Fin du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// Fin du VAO
	//glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // mode fil de fer

	// Load and create texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// définit les options de la texture actuellement liée
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// charge et génère la texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout <<red<< "Failed to load texture" << white << std::endl;
	}
	//libérer l’espace mémoire contenant l’image
	stbi_image_free(data);

	//------------------------------------------------------------------------------------------------------------------------------------
		// Load and create texture 2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << red << "Failed to load texture 2" << white << std::endl;
	}
	stbi_image_free(data);
	//--------------------------------------------------------------------------------------------------------------
	
	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;


	myShaderProgram.use(); // n’oubliez pas d’activer le shader avant de définir les variables uniformes
	glUniform1i(glGetUniformLocation(myShaderProgram.ID, "texture1"), 0);
	myShaderProgram.setInt("texture2", 1);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirectionReversed = glm::normalize(cameraPos - cameraTarget);
	
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirectionReversed));
	glm::vec3 cameraUp = glm::cross(cameraDirectionReversed, cameraRight);

	// PASS PROJECTION MATRIX
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(FIELD_OF_VIEW), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	myShaderProgram.setMat4("projection", projection);

	// boucle principal
	bool running = true;
	while (running)
	{
		// gestion des évenements
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// on stoppe le programme
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// on ajuste le viewport lorsque la fenêtre est redimensionnée
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// effacement tampons de couleur/profondeur
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Camera/view transformation
		glm::mat4 view = glm::mat4(1.0f);
		float radius = 10.0f;
		float camX = sin(clock.getElapsedTime().asSeconds()) * radius;
		float camZ = cos(clock.getElapsedTime().asSeconds()) * radius;

		// LOOK AT MATRIX
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), cameraTarget,up);
		myShaderProgram.setMat4("view", view);


		//------------------------------------------------------------------------------------------------------------
		//												Rendu du Cube
		

		glBindVertexArray(VAO);
		
		// Dessiner 10 cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			myShaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		// Dessiner les deux triangles grâce aux tableaux d'indices
		//glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0);
		
		//------------------------------------------------------------------------------------------------------------

		//fin de la trame courante (en interne, échange les deux tampons de rendu)
		window.display();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	myShaderProgram.deleteProgram();


	//window.close();
	return 0;
}
