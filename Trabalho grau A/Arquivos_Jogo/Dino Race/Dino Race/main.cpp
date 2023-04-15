#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "Layer.h"
#include "GameObject.h"
#include "Sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const float PARALLAX_RATE = 0.02f;

bool Colisao(Sprite &dino, GameObject &cacto) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = cacto.getPosX() >= dino.getPosX();
	// Collision y-axis?
	bool collisionY = cacto.getHeight() >= dino.getPosY() &&
			          cacto.getPosX() < dino.getPosX() + 3.8f;
	// Collision only if on both axes
	return collisionX && collisionY;
}

GLuint getIdImagem() {
	GLuint id_imagem;

	glGenTextures(1, &id_imagem);
	glBindTexture(GL_TEXTURE_2D, id_imagem); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return id_imagem;
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(
		800, 600, "Dino Race", NULL, NULL);

	//fullscreen
	//GLFWwindow* window = glfwCreateWindow(
	//	1600, 1200, "Dino Race", glfwGetPrimaryMonitor(), NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//Vertex da textura
	const char* vertex_shader =
		"#version 410\n"
		"layout (location = 0) in vec2 vertex_position;"
		"layout (location = 1) in vec2 texture_mapping;"
		"uniform mat4 matrix;"
		"out vec2 texture_coords;"
		"uniform float layer_z;"
		"void main() {"
		"	texture_coords = texture_mapping;"
		"   gl_Position = matrix * vec4 (vertex_position, layer_z, 1.0);"
		"}";

	//Fragment da textura
	const char* fragment_shader =
		"#version 410\n"
		"in vec2 texture_coords;"
		"uniform sampler2D sprite;"
		"uniform float offsetx;"
		"uniform float offsety;"
		"out vec4 frag_color;"
		"void main() {"
		"	vec4 texel = texture (sprite,"
		"		vec2(texture_coords.x + offsetx, texture_coords.y + offsety)); "
		"	if (texel.a < 0.5) {"
		"		discard;"
		"	}"
		"	frag_color = texel;"
		"}";

	int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	int shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);


	//Vertices das camadas
	GLfloat  vertices[] = {
		// positions    	  // texture coords
		1.0f, 1.0f, 0.0f,     1.0f, 0.0f,   // top right
		1.0f, -1.0f, 0.0f,    1.0f, 1.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 1.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f    // top left
	};

	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	//Vertices do sprite 
	GLfloat  vertices_sprite[] = {
		// positions			// texture coords
		0.0f, -0.15f, 0.0f,     1.0f, 0.0f, // top right
		0.0f, -0.98f, 0.0f,  	1.0f, 1.0f, // bottom right
	   -0.75f, -0.98f, 0.0f,	0.0f, 1.0f, // bottom left
	   -0.75f, -0.15f, 0.0f,	0.0f, 0.0f  // top left
	};

	unsigned int indices_sprite[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VAO2, VBO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_sprite), vertices_sprite, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_sprite), indices_sprite, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	//Vertices do cactus
	GLfloat  vertices_cactus[] = {
		// positions			// texture coords
		0.95f, -0.50f, 0.0f,     1.0f, 0.0f, // top right
		0.95f, -0.90f, 0.0f,  	1.0f, 1.0f,  // bottom right
		0.80f, -0.90f, 0.0f,	0.0f, 1.0f,  // bottom left
		0.80f, -0.50f, 0.0f,	0.0f, 0.0f   // top left
	};

	unsigned int indices_cactus[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VAO3, VBO3, EBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cactus), vertices_cactus, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_cactus), indices_cactus, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	//Variaveis das camadas
	GLuint idImagemTelaStart;
	GLuint idImagemTelaGameOver;
	GLuint idImagemTelaWin;
	GLuint idImagemCamadas;
	GLuint idImagemObstaculo;
	GLuint idImagemOvo;
	GLuint idImagemDino_correndo;
	GLuint idImagemDino_pulando;
	GLuint idImagemDino_morrendo;

	//Arrays de objetos
	std::vector<Layer> layers;
	std::vector<Sprite> dinos_corrida;
	std::vector<Sprite> dinos_pulo;
	std::vector<Sprite> dinos_morte;

	//Objetos
	Layer telaStart = Layer();
	Layer telaWin = Layer();
	Layer telaGameOver = Layer();
	Layer layer = Layer();
	GameObject cacto = GameObject();
	GameObject ovo = GameObject();
	Sprite dino_correndo = Sprite();
	Sprite dino_pulando = Sprite();
	Sprite dino_morrendo = Sprite();

	float layer_z = -0.53f;

	//Carregando menus do jogo
	idImagemTelaStart = getIdImagem();
	telaStart = telaStart.carregaCamadaAlpha("imagens/telas/start.png", telaStart, idImagemTelaStart, 0.0f, 0.0f);

	idImagemTelaGameOver = getIdImagem();
	telaGameOver = telaGameOver.carregaCamadaAlpha("imagens/telas/GameOver.png", telaGameOver, idImagemTelaGameOver, 0.0f, 0.0f);

	idImagemTelaWin = getIdImagem();
	telaWin = telaWin.carregaCamadaAlpha("imagens/telas/GameWin.png", telaWin, idImagemTelaWin, 0.0f, 0.0f);


	//Carregando as imagens das camadas
    idImagemCamadas = getIdImagem();
	layer = layer.carregaCamada("imagens/camadas/1_ceu.png", layer, idImagemCamadas, layer_z, 0.05f);
	layers.push_back(layer);

	idImagemCamadas = getIdImagem();
	layer = layer.carregaCamadaAlpha("imagens/camadas/2_floresta.png", layer, idImagemCamadas, layer_z - 0.02f, 0.10f);
	layers.push_back(layer);

	idImagemCamadas = getIdImagem();
	layer = layer.carregaCamadaAlpha("imagens/camadas/3_floresta.png", layer, idImagemCamadas, layer_z - 0.04f, 0.15f);
	layers.push_back(layer);

	idImagemCamadas = getIdImagem();
	layer = layer.carregaCamadaAlpha("imagens/camadas/4_floresta.png", layer, idImagemCamadas, layer_z - 0.06f, 0.20f);
	layers.push_back(layer);

	idImagemCamadas = getIdImagem();
	layer = layer.carregaCamadaAlpha("imagens/camadas/5_chao.png", layer, idImagemCamadas, layer_z - 0.08f, 0.25f);
	layers.push_back(layer);


	//Carregando a imagem do cacto
	idImagemObstaculo = getIdImagem();
	cacto = cacto.carregaObjeto("imagens/obstaculo/cactus.png", cacto, idImagemObstaculo, layer_z - 0.07f, 0.25f);

	//Carregando a imagem do ovo
	idImagemOvo = getIdImagem();
	ovo = ovo.carregaObjeto("imagens/obstaculo/ovo.png", ovo, idImagemOvo, layer_z - 0.07f, 0.25f);


	//Carregando as imagens de corrida do dino
	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo1.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo2.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo3.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo4.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo5.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo6.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo7.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);

	idImagemDino_correndo = getIdImagem();
	dino_correndo = dino_correndo.carregaSprite("imagens/sprite/correndo/dino_correndo8.png", dino_correndo, idImagemDino_correndo, layer_z - 0.08f);
	dinos_corrida.push_back(dino_correndo);


	//Carregando as imagens de pulo do dino
	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando1.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando2.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando3.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando4.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando5.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando6.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando7.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando8.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando9.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando10.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando11.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);

	idImagemDino_pulando = getIdImagem();
	dino_pulando = dino_pulando.carregaSprite("imagens/sprite/pulando/pulando12.png", dino_pulando, idImagemDino_pulando, layer_z - 0.08f);
	dinos_pulo.push_back(dino_pulando);


	//Carregando as imagens de morte do dino
	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo1.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo2.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo3.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo4.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo5.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo6.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo7.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);

	idImagemDino_morrendo = getIdImagem();
	dino_morrendo = dino_morrendo.carregaSprite("imagens/sprite/morrendo/morrendo8.png", dino_morrendo, idImagemDino_morrendo, layer_z - 0.08f);
	dinos_morte.push_back(dino_morrendo);


	//Variaveis de controle do jogo
	int controle_sprites_dino_corrida = 0;
	int controle_sprites_dino_pulo = 0;
	int controle_sprites_dino_morte = 0;
	int vidas = 3;
	int qtdCactos = 0;
	float y_pulo = 0.0f;

	//Variaveis de controle da animação
	static double previousSeconds = 0.0;
	double currentSeconds;
	double elapsedSeconds;
	static double previousSeconds2 = 0.0;
	double currentSeconds2;
	double elapsedSeconds2;
	static double previousSeconds3 = 0.0;
	double currentSeconds3;
	double elapsedSeconds3;

	//Variáveis de controle das ações do dino
	bool correndo = true;
	bool pulando = false;
	bool morrendo = false;
	bool morreuAgora = false;
	float movimento_cacto = 0.0f;
	
	//Variáveis de progressão do jogo
	bool gameOver = false;
	bool gameWin = false;
	bool gameStart = false;
	

	//Ajustar a pos Y do dino para cálculo da colisão no pulo
	dino_correndo.setPosY((float)cacto.getHeight() - 3.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);

		glUseProgram(shader_programme);

		glm::mat4 matrix = glm::mat4(1.0f);
		unsigned int transformLoc = glGetUniformLocation(shader_programme, "matrix");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

		if ((!gameStart) && (!gameOver) && (!gameWin)) {
			//Tela Start
			glBindVertexArray(VAO);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), telaStart.getTX());
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), telaStart.getTY());
			glUniform1f(
				glGetUniformLocation(shader_programme, "layer_z"), telaStart.getTZ());
			// bind Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, telaStart.getTID());
			glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		if (gameOver) {
			//Tela game over
			glBindVertexArray(VAO);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), telaGameOver.getTX());
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), telaGameOver.getTY());
			glUniform1f(
				glGetUniformLocation(shader_programme, "layer_z"), telaGameOver.getTZ());
			// bind Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, telaGameOver.getTID());
			glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		if (gameWin) {
			//Tela Win
			glBindVertexArray(VAO);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), telaWin.getTX());
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), telaWin.getTY());
			glUniform1f(
				glGetUniformLocation(shader_programme, "layer_z"), telaWin.getTZ());
			// bind Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, telaWin.getTID());
			glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		if (gameStart) {
			//Camadas
			glBindVertexArray(VAO);
			for (int i = 0; i < (int)layers.size(); i++) {
				if (!morrendo) {
					layers.at(i).setTX(layers.at(i).getTX() + (layers.at(i).getScrollRateX() * PARALLAX_RATE));
				}
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsetx"), layers.at(i).getTX());
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsety"), layers.at(i).getTY());
				glUniform1f(
					glGetUniformLocation(shader_programme, "layer_z"), layers.at(i).getTZ());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, layers.at(i).getTID());
				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		
	
			//Cacto
			if ((movimento_cacto <= -2.5f) && (movimento_cacto != 0.0f)) {
				movimento_cacto = 0.0f;

				cacto.setPosX(cacto.getTX());
				dino_correndo.setPosY((float)cacto.getHeight() - 3.0f);

				matrix = glm::mat4(1.0f);
				matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
				transformLoc = glGetUniformLocation(shader_programme, "matrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

				//contabiliza a qtd de cactos exibidos
				qtdCactos = qtdCactos + 1;
			}
			else {
				matrix = glm::mat4(1.0f);
				matrix = glm::translate(matrix, glm::vec3(movimento_cacto, 0.0f, 0.0f));
				transformLoc = glGetUniformLocation(shader_programme, "matrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
			}
			if (qtdCactos < 10) {
				glBindVertexArray(VAO3);
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsetx"), cacto.getTX());
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsety"), cacto.getTY());
				glUniform1f(
					glGetUniformLocation(shader_programme, "layer_z"), cacto.getTZ());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cacto.getTID());
				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				//verifica colisão
				cacto.setPosX(cacto.getPosX() + 3.8f);
				if (!morrendo) {
					morrendo = Colisao(dino_correndo, cacto);
					//Ajusta próximo movimento
					movimento_cacto = movimento_cacto - 0.01f;
				}
			}
			else {
				glBindVertexArray(VAO3);
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsetx"), ovo.getTX());
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsety"), ovo.getTY());
				glUniform1f(
					glGetUniformLocation(shader_programme, "layer_z"), ovo.getTZ());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ovo.getTID());
				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				cacto.setPosX(cacto.getPosX() + 3.8f);
				if (Colisao(dino_correndo, cacto)) {
					gameWin = true;
					gameStart = false;
				} else {
					//Ajusta próximo movimento
					movimento_cacto = movimento_cacto - 0.01f;
				}
			}

		
			//Sprite dino
			if (correndo) {
				currentSeconds = glfwGetTime();
				elapsedSeconds = currentSeconds - previousSeconds;

				matrix = glm::mat4(1.0f);
				transformLoc = glGetUniformLocation(shader_programme, "matrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

				glBindVertexArray(VAO2);
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsetx"), dinos_corrida.at(controle_sprites_dino_corrida).getTX());
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsety"), dinos_corrida.at(controle_sprites_dino_corrida).getTY());
				glUniform1f(
					glGetUniformLocation(shader_programme, "layer_z"), dinos_corrida.at(controle_sprites_dino_corrida).getTZ());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, dinos_corrida.at(controle_sprites_dino_corrida).getTID());
				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				//Controle transição sprites corrida
				if (elapsedSeconds >= 0.10) {
					previousSeconds = currentSeconds;
					controle_sprites_dino_corrida++;
					if (controle_sprites_dino_corrida == 8) {
						controle_sprites_dino_corrida = 0;
					}
				}
			}
		
			if (pulando) {
				currentSeconds2 = glfwGetTime();
				elapsedSeconds2 = currentSeconds2 - previousSeconds2;

				matrix = glm::mat4(1.0f);
				matrix = glm::translate(matrix, glm::vec3(0.0f, y_pulo, 0.0f));
				transformLoc = glGetUniformLocation(shader_programme, "matrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

				glBindVertexArray(VAO2);
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsetx"), dinos_pulo.at(controle_sprites_dino_pulo).getTX());
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsety"), dinos_pulo.at(controle_sprites_dino_pulo).getTY());
				glUniform1f(
					glGetUniformLocation(shader_programme, "layer_z"), dinos_pulo.at(controle_sprites_dino_pulo).getTZ());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, dinos_pulo.at(controle_sprites_dino_pulo).getTID());
				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				//Verifica colisão
				morrendo = Colisao(dino_correndo, cacto);
				
				//Controle pulo
				if (elapsedSeconds2 >= 0.08) {
					previousSeconds2 = currentSeconds2;
					if (controle_sprites_dino_pulo < 6) {
						y_pulo = y_pulo + 0.14f;
						dino_correndo.setPosY(dino_correndo.getPosY() + 1.0f);
					}
					else {
						y_pulo = y_pulo - 0.14f;
						dino_correndo.setPosY(dino_correndo.getPosY() - 1.0f);
					}
					controle_sprites_dino_pulo++;
					if (controle_sprites_dino_pulo == 12) {
						controle_sprites_dino_pulo = 0;
						correndo = true;
						pulando = false;
					}
				}
			}
		
			if (morrendo) {
				correndo = false;
				pulando = false;

				currentSeconds3 = glfwGetTime();
				elapsedSeconds3 = currentSeconds3 - previousSeconds3;

				matrix = glm::mat4(1.0f);
				transformLoc = glGetUniformLocation(shader_programme, "matrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

				glBindVertexArray(VAO2);
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsetx"), dinos_morte.at(controle_sprites_dino_morte).getTX());
				glUniform1f(
					glGetUniformLocation(shader_programme, "offsety"), dinos_morte.at(controle_sprites_dino_morte).getTY());
				glUniform1f(
					glGetUniformLocation(shader_programme, "layer_z"), dinos_morte.at(controle_sprites_dino_morte).getTZ());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, dinos_morte.at(controle_sprites_dino_morte).getTID());
				glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				//Controle transição sprites de morte
				if (elapsedSeconds3 >= 0.10) {
					previousSeconds3 = currentSeconds3;
					controle_sprites_dino_morte++;
					if (controle_sprites_dino_morte == 8) {
						controle_sprites_dino_morte = 0;
						correndo = true;
						pulando = false;
						morrendo = false;
						morreuAgora = false;
						if (!gameOver) {
							y_pulo = 0.0f;
							movimento_cacto = 0.0f;
							cacto.setPosX(cacto.getTX());
							controle_sprites_dino_pulo = 0;
						}
					}
				}
			}
		}

		if (!gameOver) {
			//Captura o pressionamento do espaço para o pulo
			int statePulo = glfwGetKey(window, GLFW_KEY_SPACE);
			if ((statePulo == GLFW_PRESS) && (!morrendo) && (gameStart)) {
				correndo = false;
				pulando = true;
			}

			//Captura o pressionamento do ENTER para começar
			int stateStart = glfwGetKey(window, GLFW_KEY_ENTER);
			if ((stateStart == GLFW_PRESS) && (!gameStart) && (!gameWin)) {
				gameStart = true;
			}

			//Verificar se houve colisão e contabilizar mortes
			if ((morrendo) && (!morreuAgora)) {
				morreuAgora = true;
				vidas = vidas - 1;
			}

			//Verificar se houve game over
			if ((vidas == 0) && (!morrendo)) {
				gameOver = true;
				gameStart = false;
			}
		}

		glfwSwapBuffers(window);
	}

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}