#include "TileMapView.h"
#include "DiamondView.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//Variaveis
const int WIDTH = 800;
const int HEIGHT = 600;

const float th = 0.125f;
const float tw = th * 2;

float x = 0.0f;
float y = 0.0f;
float x_cursor = 0.0f;
float y_cursor = 0.0f;
float x_origem = 0.0f;
float y_origem = 0.0f;
float x_destino = 0.0f;
float y_destino = 0.0f;
float x_caminho = 0.0f;
float y_caminho = 0.0f;
float colTile = 0.0f;
float rowTile = 0.0f;
float colMarcador = 0.0f;
float rowMarcador = 0.0f;

//Colunae linha do cursor
int linha_cursor = 0;
int coluna_cursor = 0;
//Coluna e linha da origem da peça
int linha_peca_origem = -1;
int coluna_peca_origem = -1;
//Coluna e linha do destino da peça
int linha_peca_destino = -1;
int coluna_peca_destino = -1;
//Coluna e linha da peça que pode morrer
int linha_peca_capturada_norte = -1;
int coluna_peca_capturada_norte = -1;
int linha_peca_capturada_sul = -1;
int coluna_peca_capturada_sul = -1;
int linha_peca_capturada_leste = -1;
int coluna_peca_capturada_leste = -1;
int linha_peca_capturada_oeste = -1;
int coluna_peca_capturada_oeste = -1;
//Coluna e linha que a peça deve ir para matar outra
int linha_peca_capturar_norte = -1;
int coluna_peca_capturar_norte = -1;
int linha_peca_capturar_sul = -1;
int coluna_peca_capturar_sul = -1;
int linha_peca_capturar_leste = -1;
int coluna_peca_capturar_leste = -1;
int linha_peca_capturar_oeste = -1;
int coluna_peca_capturar_oeste = -1;
//IDs
int IDImagemTabuleiro = 0;
int IDImagemPecas = 0;
int IDPecaClique = 0;
//Qtd peças
int qtd_pecas_brancas = 12;
int qtd_pecas_pretas = 12;

bool cliqueMouse = false;
bool pecaAndando = false;
bool destinoX = false;
bool destinoY = false;
bool vezBrancas = true;
bool vezPretas = false;
bool pecaCapturada = false;
bool jogoIniciado = false;
bool jogoFinalizado = false;

//0 - Escuro  |  1 - Claro
const unsigned char diamondMapTabuleiro[8][8] = { 0, 1, 0, 1, 0, 1, 0, 1,
												  1, 0, 1, 0, 1, 0, 1, 0,
												  0, 1, 0, 1, 0, 1, 0, 1,
												  1, 0, 1, 0, 1, 0, 1, 0,
												  0, 1, 0, 1, 0, 1, 0, 1,
												  1, 0, 1, 0, 1, 0, 1, 0,
												  0, 1, 0, 1, 0, 1, 0, 1,
												  1, 0, 1, 0, 1, 0, 1, 0 };

//0 - Caminho andável 
//1 - caminho não andável
unsigned char diamondMapColisao[8][8] = { 0, 1, 0, 1, 0, 1, 0, 1,
										  1, 0, 1, 0, 1, 0, 1, 0,
										  0, 1, 0, 1, 0, 1, 0, 1,
										  1, 0, 1, 0, 1, 0, 1, 0,
										  0, 1, 0, 1, 0, 1, 0, 1,
										  1, 0, 1, 0, 1, 0, 1, 0,
										  0, 1, 0, 1, 0, 1, 0, 1,
										  1, 0, 1, 0, 1, 0, 1, 0 };

//Mapa padrão de jogadas possiveis, onde 1 será onde não é possível mover a peça
const unsigned char diamondMapJogadasPadrao[8][8] = { 1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1,
													  1, 1, 1, 1, 1, 1, 1, 1 };

//Mapa alterado em tempo de execução, para mostrar com 0 as jogadas possíveis da peça selecionada
unsigned char diamondMapJogadasValidacao[8][8];

//Mapa padrão das peças
const unsigned char diamondMapPecasPadrao[8][8] = { 2, 5, 2, 5, 5, 5, 0, 5,
													5, 2, 5, 5, 5, 0, 5, 0,
													2, 5, 2, 5, 5, 5, 0, 5,
													5, 2, 5, 5, 5, 0, 5, 0,
													2, 5, 2, 5, 5, 5, 0, 5,
													5, 2, 5, 5, 5, 0, 5, 0,
													2, 5, 2, 5, 5, 5, 0, 5,
													5, 2, 5, 5, 5, 0, 5, 0 };

//0 - Preta comum  |  1 - Preta dama
//2 - Branca comum |  3 - Branca dama
//4 e 5 - Tiles transparentes
unsigned char diamondMapPecas[8][8] = { 2, 5, 2, 5, 5, 5, 0, 5,
										5, 2, 5, 5, 5, 0, 5, 0,
										2, 5, 2, 5, 5, 5, 0, 5,
										5, 2, 5, 5, 5, 0, 5, 0,
										2, 5, 2, 5, 5, 5, 0, 5,
										5, 2, 5, 5, 5, 0, 5, 0,
										2, 5, 2, 5, 5, 5, 0, 5,
										5, 2, 5, 5, 5, 0, 5, 0 };

TileMapView *tv = new DiamondView();

//Métodos da classe Main
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

void carregaImagem(const char* imagem, int idTextura) {
	int width, height, nrChannels;
	unsigned char *data;

	data = stbi_load(imagem, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

//Calcular área do triângulo
float areaTriangulo(float *triangulo) {
	return ((triangulo[2] - triangulo[0]) * (triangulo[5] - triangulo[1])
		- (triangulo[4] - triangulo[0]) * (triangulo[3] - triangulo[1])
		) / 2;
}

//Validar colisão do ponto clicado
bool collision(float *ABD, float ponto[]) {
	float *ApB = new float[6];
	float *pBD = new float[6];
	float *ApD = new float[6];

	ApB[0] = ABD[0];				ApB[1] = ABD[1];
	ApB[2] = ponto[0];		        ApB[3] = ponto[1];
	ApB[4] = ABD[4];		        ApB[5] = ABD[5];

	pBD[0] = ponto[0];				pBD[1] = ponto[1];
	pBD[2] = ABD[2];		        pBD[3] = ABD[3];
	pBD[4] = ABD[4];		        pBD[5] = ABD[5];

	ApD[0] = ABD[0];				ApD[1] = ABD[1];
	ApD[2] = ponto[0];		        ApD[3] = ponto[1];
	ApD[4] = ABD[4];		        ApD[5] = ABD[5];

	if (areaTriangulo(ABD) == areaTriangulo(ApB) + areaTriangulo(pBD) + areaTriangulo(ApD)) {
		return true;
	}
	else {
		return false;
	}
}

//Tratamento do mouse
void mouse(float mx, float my) {
	//Conversão de mx e my para ficar entre -1 e 1(vértices)
	mx = ((mx / WIDTH) - 0.5f) * 2;
	my = ((my / HEIGHT) - 0.5f) * -2;

	// 1 - executar cálculo do mousemap
	int c, r;
	tv->MouseMap(c, r, tw, th, mx + 1.0f, my);

	// 2 - normalizar clique
	float x0, y0;
	tv->calculoPosicaoDesenho(c, r, tw, th, x0, y0);

	float point[] = { mx, my };

	//Triângulo ABC
	float *abc = new float[6];

	//Definir metade da direita ou da esquerda
	bool left = mx < (x0 + tw / 2.0f);
	if (left) {
		abc[0] = x0;				    abc[1] = y0 + th / 2.0f;
		abc[2] = x0 + tw / 2.0f;		abc[3] = y0 + th;
		abc[4] = x0 + tw / 2.0f;		abc[5] = y0;
	}
	else {
		abc[0] = x0 + tw / 2.0f;		abc[1] = y0;
		abc[2] = x0 + tw / 2.0f;		abc[3] = y0 + th;
		abc[4] = x0 + tw;        		abc[5] = y0 + th / 2.0f;
	}

	// 3 - testar se o ponto colide com losango definido a
	// partir do ponto x0, y0.
	bool collide = collision(abc, point);
	if (!collide) {
		// 3.1 - não pertencendo a alguma metade do losango
		// executar tile walking para o vizinho correto
		bool topo = my > (y0 + th / 2.0f);
		if (topo) {
			if (left) {
				tv->TileWalking(c, r, DIRECTION_NORTHWEST);
			}
			else {
				tv->TileWalking(c, r, DIRECTION_NORTHEAST);
			}
		}
		else {
			if (left) {
				tv->TileWalking(c, r, DIRECTION_SOUTHWEST);
			}
			else {
				tv->TileWalking(c, r, DIRECTION_SOUTHEAST);
			}
		}
	}
	coluna_cursor = c; linha_cursor = r;
}

//Configura a captura da peça adversária detectada
void configurarPossivelCaptura(int col_capturada, int linha_capturada, int col_capturar, int linha_capturar, int direcao) {
	if (direcao == DIRECTION_NORTH) {
		linha_peca_capturada_norte = linha_capturada;
		coluna_peca_capturada_norte = col_capturada;

		linha_peca_capturar_norte = linha_capturar;
		coluna_peca_capturar_norte = col_capturar;
	} else if (direcao == DIRECTION_SOUTH) {
		linha_peca_capturada_sul = linha_capturada;
		coluna_peca_capturada_sul = col_capturada;

		linha_peca_capturar_sul = linha_capturar;
		coluna_peca_capturar_sul = col_capturar;
	} else if (direcao == DIRECTION_EAST) {
		linha_peca_capturada_leste = linha_capturada;
		coluna_peca_capturada_leste = col_capturada;

		linha_peca_capturar_leste = linha_capturar;
		coluna_peca_capturar_leste = col_capturar;
	} else if (direcao == DIRECTION_WEST) {
		linha_peca_capturada_oeste = linha_capturada;
		coluna_peca_capturada_oeste = col_capturada;

		linha_peca_capturar_oeste = linha_capturar;
		coluna_peca_capturar_oeste = col_capturar;
	}
}

//Valida e configura a captura quando for possível jogada múltipla
void validarColisao_JogadaMultipla(int col_possivel, int linha_possivel, bool &continuarJogada, int IDPeca, int direcao) {
	int inimigo_comum;
	int inimigo_dama;
	int col_peca_capturada;
	int linha_peca_capturada;

	if ((IDPeca == 0) || (IDPeca == 1)) {
		inimigo_comum = 2;
		inimigo_dama = 3;
	} else {
		inimigo_comum = 0;
		inimigo_dama = 1;
	}

	tv->TileWalking(col_possivel, linha_possivel, direcao);
	if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && ((diamondMapPecas[linha_possivel][col_possivel] == inimigo_comum) || (diamondMapPecas[linha_possivel][col_possivel] == inimigo_dama))) {
		linha_peca_capturada = linha_possivel;
		col_peca_capturada = col_possivel;

		tv->TileWalking(col_possivel, linha_possivel, direcao);
		if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && (diamondMapPecas[linha_possivel][col_possivel] == 5)) {
			diamondMapJogadasValidacao[linha_possivel][col_possivel] = 0;
			configurarPossivelCaptura(col_peca_capturada, linha_peca_capturada, col_possivel, linha_possivel, direcao);
			continuarJogada = true;
		}
	}
}

//Após captura, verificar possível captura extra
bool verificarPossivelJogadaMultipla(int linha_origem, int col_origem, int IDPeca) {
	int col_possivel;
	int linha_possivel;
	bool continuarJogada = false;

	//Limpa o mapa das jogadas possíveis
	for (int i = 0; i < 8; i++) {	//Linhas
		for (int j = 0; j < 8; j++) {  //Colunas
			diamondMapJogadasValidacao[i][j] = diamondMapJogadasPadrao[i][j];
		}
	}

	linha_possivel = linha_origem;
	col_possivel = col_origem;

	validarColisao_JogadaMultipla(col_possivel, linha_possivel, continuarJogada, IDPeca, DIRECTION_NORTH);
	validarColisao_JogadaMultipla(col_possivel, linha_possivel, continuarJogada, IDPeca, DIRECTION_SOUTH);
	validarColisao_JogadaMultipla(col_possivel, linha_possivel, continuarJogada, IDPeca, DIRECTION_EAST);
	validarColisao_JogadaMultipla(col_possivel, linha_possivel, continuarJogada, IDPeca, DIRECTION_WEST);

	return continuarJogada;
}

void marcarCaminhoPossivel(int col_possivel, int linha_possivel, int IDPeca, int direcao) {
	int inimigo_comum;
	int inimigo_dama;
	int col_peca_capturada;
	int linha_peca_capturada;

	if ((IDPeca == 0) || (IDPeca == 1)) {
		inimigo_comum = 2;
		inimigo_dama = 3;
	} else {
		inimigo_comum = 0;
		inimigo_dama = 1;
	}

	//Peças comuns
	if ((IDPeca == 0) || (IDPeca == 2)) {
		tv->TileWalking(col_possivel, linha_possivel, direcao);
		if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && (diamondMapPecas[linha_possivel][col_possivel] == 5)) {
			diamondMapJogadasValidacao[linha_possivel][col_possivel] = 0;
		}
		else if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && ((diamondMapPecas[linha_possivel][col_possivel] == inimigo_comum) || (diamondMapPecas[linha_possivel][col_possivel] == inimigo_dama))) {
			linha_peca_capturada = linha_possivel;
			col_peca_capturada = col_possivel;

			tv->TileWalking(col_possivel, linha_possivel, direcao);
			if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && (diamondMapPecas[linha_possivel][col_possivel] == 5)) {
				diamondMapJogadasValidacao[linha_possivel][col_possivel] = 0;
				configurarPossivelCaptura(col_peca_capturada, linha_peca_capturada, col_possivel, linha_possivel, direcao);
			}
		}
	}
	//Peças dama
	else {
		tv->TileWalking(col_possivel, linha_possivel, direcao);
		while ((diamondMapColisao[linha_possivel][col_possivel] == 0) && (diamondMapPecas[linha_possivel][col_possivel] == 5)) {
			diamondMapJogadasValidacao[linha_possivel][col_possivel] = 0;
			tv->TileWalking(col_possivel, linha_possivel, direcao);
		}
		if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && ((diamondMapPecas[linha_possivel][col_possivel] == inimigo_comum) || (diamondMapPecas[linha_possivel][col_possivel] == inimigo_dama))) {
			linha_peca_capturada = linha_possivel;
			col_peca_capturada = col_possivel;

			tv->TileWalking(col_possivel, linha_possivel, direcao);
			if ((diamondMapColisao[linha_possivel][col_possivel] == 0) && (diamondMapPecas[linha_possivel][col_possivel] == 5)) {
				diamondMapJogadasValidacao[linha_possivel][col_possivel] = 0;
				configurarPossivelCaptura(col_peca_capturada, linha_peca_capturada, col_possivel, linha_possivel, direcao);
			}
		}
	}
}

void identificarJogadasPossiveis(int linha_origem, int col_origem, int IDPeca) {
	int col_possivel;
	int linha_possivel;
	int col_captura_atras;
	int linha_captura_atras;

	linha_possivel = linha_origem;
	col_possivel = col_origem;

	if (IDPeca == 0) {
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_SOUTH);
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_WEST);

		//Verificar possível captura atrás
		linha_captura_atras = linha_origem;
		col_captura_atras = col_origem;
		tv->TileWalking(col_captura_atras, linha_captura_atras, DIRECTION_NORTH);
		if ((diamondMapPecas[linha_captura_atras][col_captura_atras] == 2) || (diamondMapPecas[linha_captura_atras][col_captura_atras] == 3)) {
			marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_NORTH);
		}

		linha_captura_atras = linha_origem;
		col_captura_atras = col_origem;
		tv->TileWalking(col_captura_atras, linha_captura_atras, DIRECTION_EAST);
		if ((diamondMapPecas[linha_captura_atras][col_captura_atras] == 2) || (diamondMapPecas[linha_captura_atras][col_captura_atras] == 3)) {
			marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_EAST);
		}
	} else if (IDPeca == 2) {
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_NORTH);
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_EAST);

		//Verificar possível captura atrás
		linha_captura_atras = linha_origem;
		col_captura_atras = col_origem;
		tv->TileWalking(col_captura_atras, linha_captura_atras, DIRECTION_SOUTH);
		if ((diamondMapPecas[linha_captura_atras][col_captura_atras] == 0) || (diamondMapPecas[linha_captura_atras][col_captura_atras] == 1)) {
			marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_SOUTH);
		}

		linha_captura_atras = linha_origem;
		col_captura_atras = col_origem;
		tv->TileWalking(col_captura_atras, linha_captura_atras, DIRECTION_WEST);
		if ((diamondMapPecas[linha_captura_atras][col_captura_atras] == 0) || (diamondMapPecas[linha_captura_atras][col_captura_atras] == 1)) {
			marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_WEST);
		}
	} else {
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_NORTH);
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_SOUTH);
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_EAST);
		marcarCaminhoPossivel(col_possivel, linha_possivel, IDPeca, DIRECTION_WEST);
	}
}

void restart() {
	x = 0.0f;
	y = 0.0f;
	x_cursor = 0.0f;
	y_cursor = 0.0f;
	x_origem = 0.0f;
	y_origem = 0.0f;
	x_destino = 0.0f;
	y_destino = 0.0f;
	x_caminho = 0.0f;
	y_caminho = 0.0f;
	colTile = 0.0f;
	rowTile = 0.0f;
	colMarcador = 0.0f;
	rowMarcador = 0.0f;

	//Coluna e linha do cursor
	linha_cursor = 0;
	coluna_cursor = 0;
	//Coluna e linha da origem da peça
	linha_peca_origem = -1;
	coluna_peca_origem = -1;
	//Coluna e linha do destino da peça
	linha_peca_destino = -1;
	coluna_peca_destino = -1;
	//Coluna e linha da peça que pode morrer
	linha_peca_capturada_norte = -1;
	coluna_peca_capturada_norte = -1;
	linha_peca_capturada_sul = -1;
	coluna_peca_capturada_sul = -1;
	linha_peca_capturada_leste = -1;
	coluna_peca_capturada_leste = -1;
	linha_peca_capturada_oeste = -1;
	coluna_peca_capturada_oeste = -1;
	//Coluna e linha que a peça deve ir para matar outra
	linha_peca_capturar_norte = -1;
	coluna_peca_capturar_norte = -1;
	linha_peca_capturar_sul = -1;
	coluna_peca_capturar_sul = -1;
	linha_peca_capturar_leste = -1;
	coluna_peca_capturar_leste = -1;
	linha_peca_capturar_oeste = -1;
	coluna_peca_capturar_oeste = -1;
	//IDs
	IDImagemTabuleiro = 0;
	IDImagemPecas = 0;
	IDPecaClique = 0;
	//Qtd peças
	qtd_pecas_brancas = 12;
	qtd_pecas_pretas = 12;

	cliqueMouse = false;
	pecaAndando = false;
	destinoX = false;
	destinoY = false;
	vezBrancas = true;
	vezPretas = false;
	pecaCapturada = false;
	jogoIniciado = false;
	jogoFinalizado = false;

	//Limpa o mapa das jogadas possíveis
	for (int i = 0; i < 8; i++) {	//Linhas
		for (int j = 0; j < 8; j++) {  //Colunas
			diamondMapJogadasValidacao[i][j] = diamondMapJogadasPadrao[i][j];
		}
	}

	//Reseta o mapa das peças
	for (int i = 0; i < 8; i++) {	//Linhas
		for (int j = 0; j < 8; j++) {  //Colunas
			diamondMapPecas[i][j] = diamondMapPecasPadrao[i][j];
		}
	}
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(
		WIDTH, HEIGHT, "Damas", NULL, NULL);
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
		"uniform float x;"
		"uniform float y;"
		"out vec4 frag_color;"
		"void main () {"
		"	vec2 tc = vec2((texture_coords.x + offsetx), (texture_coords.y + offsety));"
		"	vec4 texel = texture(sprite, tc);"
		"	if (texel.a < 0.5) {"
		"		discard;"
		"	}"
		"	frag_color = texel;"
		"}";

	// identifica vs e o associa com vertex_shader
	int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	int shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	//losango para diamondMap, mapeado no centro da tela
	GLfloat  vertices_tabuleiro[] = {
		// positions			 // texture coords
		 tw / 2, th,     0.0f,   0.25f, 0.0f,  // top right
		 tw,     th / 2, 0.0f,   0.5f, 0.5f,   // bottom right
		 tw / 2, 0.0f,   0.0f,	 0.25f, 1.0f,  // bottom left
		 0.0f,   th / 2, 0.0f,	 0.0f, 0.5f    // top left
	};

	unsigned int indices_tabuleiro[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VBO_Tabuleiro, VAO_Tabuleiro, EBO_Tabuleiro;
	glGenVertexArrays(1, &VAO_Tabuleiro);
	glGenBuffers(1, &VBO_Tabuleiro);
	glGenBuffers(1, &EBO_Tabuleiro);

	glBindVertexArray(VAO_Tabuleiro);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tabuleiro);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tabuleiro), vertices_tabuleiro, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Tabuleiro);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_tabuleiro), indices_tabuleiro, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//losango para as peças
	GLfloat  vertices_pecas[] = {
		// positions		        // texture coords
		tw / 2,  th,      0.0f,     0.25f, 0.0f,   // top right
		tw,      th / 2,  0.0f,     0.5f,  0.166f,  // bottom right
		tw / 2,  0.0f,    0.0f,	    0.25f, 0.33f,  // bottom left
		0.0f,    th / 2,  0.0f,	    0.0f,  0.166f   // top left
	};

	unsigned int indices_pecas[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VBO_Pecas, VAO_Pecas, EBO_Pecas;
	glGenVertexArrays(1, &VAO_Pecas);
	glGenBuffers(1, &VBO_Pecas);
	glGenBuffers(1, &EBO_Pecas);

	glBindVertexArray(VAO_Pecas);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pecas);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pecas), vertices_pecas, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Pecas);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_pecas), indices_pecas, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//losango para o cursor
	GLfloat  vertices_cursor[] = {
		// positions			 // texture coords
		 tw / 2, th,     0.0f,   0.25f, 0.0f,  // top right
		 tw,     th / 2, 0.0f,   0.5f, 0.25f,   // bottom right
		 tw / 2, 0.0f,   0.0f,	 0.25f, 0.5f,  // bottom left
		 0.0f,   th / 2, 0.0f,	 0.0f, 0.25f    // top left
	};

	unsigned int indices_cursor[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VBO_Cursor, VAO_Cursor, EBO_Cursor;
	glGenVertexArrays(1, &VAO_Cursor);
	glGenBuffers(1, &VBO_Cursor);
	glGenBuffers(1, &EBO_Cursor);

	glBindVertexArray(VAO_Cursor);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cursor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cursor), vertices_cursor, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Cursor);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_cursor), indices_cursor, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Vertices da tela inicial e de vitórias
	GLfloat  vertices_menus[] = {
		// positions    	  // texture coords
		1.0f, 1.0f, 0.0f,     1.0f, 0.0f,   // top right
		1.0f, -1.0f, 0.0f,    1.0f, 1.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 1.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f    // top left
	};

	unsigned int indices_menus[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	unsigned int VBO_Menus, VAO_Menus, EBO_Menus;
	glGenVertexArrays(1, &VAO_Menus);
	glGenBuffers(1, &VBO_Menus);
	glGenBuffers(1, &EBO_Menus);

	glBindVertexArray(VAO_Menus);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Menus);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_menus), vertices_menus, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Menus);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_menus), indices_menus, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Carregar tile Mapa
	GLuint ID_tileMapa = getIdImagem();
	carregaImagem("imagens/jogo/TileSet_Tabuleiro.png", ID_tileMapa);

	//Carregar tile Peças
	GLuint ID_tilePecas = getIdImagem();
	carregaImagem("imagens/jogo/TileSet_Pecas.png", ID_tilePecas);

	//Carregar tile Marcadores
	GLuint ID_tileMarcador = getIdImagem();
	carregaImagem("imagens/jogo/TileSet_Marcadores.png", ID_tileMarcador);

	//Carregar tela inicial
	GLuint ID_TelaInicial = getIdImagem();
	carregaImagem("imagens/telas/tela_inicial/tela_inicial.png", ID_TelaInicial);

	//Carregar tela vitória brancas
	GLuint ID_TelaVitoriaBrancas = getIdImagem();
	carregaImagem("imagens/telas/tela_vitoria/vitoria_brancas.png", ID_TelaVitoriaBrancas);

	//Carregar tela vitória pretas
	GLuint ID_TelaVitoriaPretas = getIdImagem();
	carregaImagem("imagens/telas/tela_vitoria/vitoria_pretas.png", ID_TelaVitoriaPretas);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);

		glUseProgram(shader_programme);

		//Matriz identidade
		glm::mat4 matrix = glm::mat4(1.0f);
		unsigned int transformLoc = glGetUniformLocation(shader_programme, "matrix");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

		if (qtd_pecas_pretas == 0) {
			jogoFinalizado = true;
			//Tela vitória brancas
			glBindVertexArray(VAO_Menus);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), 1);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ID_TelaVitoriaBrancas);
			glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		} else if (qtd_pecas_brancas == 0) {
			jogoFinalizado = true;
			//Tela vitória pretas
			glBindVertexArray(VAO_Menus);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), 1);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ID_TelaVitoriaPretas);
			glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		} else if (!jogoIniciado) {
			//Tela Start
			glBindVertexArray(VAO_Menus);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), 1);
			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ID_TelaInicial);
			glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		} else {
			//Define VAO Tabuleiro
			glBindVertexArray(VAO_Tabuleiro);

			for (int i = 0; i < 8; i++) {	//Linhas
				for (int j = 0; j < 8; j++) {  //Colunas
					tv->calculoPosicaoDesenho(j, i, tw, th, x, y);

					//translação do tile(em x e y baseado em c e r)
					glm::mat4 matrix = glm::mat4(1.0f);
					matrix = glm::translate(matrix, glm::vec3(x, y, 0.0f));
					unsigned int transformLoc = glGetUniformLocation(shader_programme, "matrix");
					glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

					//Definição de textura a ser utilizada, segundo o tilemap
					IDImagemTabuleiro = diamondMapTabuleiro[i][j];
					colTile = IDImagemTabuleiro % 2;
					rowTile = IDImagemTabuleiro / 1;

					//define coordenadas x e y de mapeamento de textura 
					glUniform1f(
						glGetUniformLocation(shader_programme, "offsetx"), colTile / 2);
					glUniform1f(
						glGetUniformLocation(shader_programme, "offsety"), rowTile / 1);

					//Define textura Tabuleiro
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, ID_tileMapa);
					glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);

					//Desenha losango para o tile
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
			glBindVertexArray(0);

			//Define textura Peças
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ID_tilePecas);
			glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);

			//Define VAO Peças
			glBindVertexArray(VAO_Pecas);

			for (int i = 0; i < 8; i++) {	//Linhas
				for (int j = 0; j < 8; j++) {  //Colunas
					tv->calculoPosicaoDesenho(j, i, tw, th, x, y);

					//Controlar movimentação da peça
					if ((i == linha_peca_origem) && (j == coluna_peca_origem) && (coluna_peca_destino >= 0) && (linha_peca_destino >= 0) && pecaAndando) {
						if (!destinoX) {
							if (x_origem < x_destino) {
								x_caminho += 0.002f;
								if (x_caminho >= x_destino) {
									destinoX = true;
								}
							}
							else if (x_origem != x_destino) {
								x_caminho -= 0.002f;
								if (x_caminho <= x_destino) {
									destinoX = true;
								}
							}
						}
						if (!destinoY) {
							if (y_origem < y_destino) {
								y_caminho += 0.002f;
								if (y_caminho >= y_destino) {
									destinoY = true;
								}
							}
							else if (y_origem != y_destino) {
								y_caminho -= 0.002f;
								if (y_caminho <= y_destino) {
									destinoY = true;
								}
							}
						}

						//translação do tile(em x e y baseado em c e r)
						glm::mat4 matrix = glm::mat4(1.0f);
						matrix = glm::translate(matrix, glm::vec3(x_caminho, y_caminho, -0.3f));
						unsigned int transformLoc = glGetUniformLocation(shader_programme, "matrix");
						glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

					}
					else {
						//translação do tile(em x e y baseado em c e r)
						glm::mat4 matrix = glm::mat4(1.0f);
						matrix = glm::translate(matrix, glm::vec3(x, y, -0.1f));
						unsigned int transformLoc = glGetUniformLocation(shader_programme, "matrix");
						glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
					}

					//Quando peça chegou no destino
					if (destinoX || destinoY) {
						//Detectando captura da peça adversária
						if ((coluna_peca_destino == coluna_peca_capturar_norte) && (linha_peca_destino == linha_peca_capturar_norte)) {
							if ((diamondMapPecas[linha_peca_capturada_norte][coluna_peca_capturada_norte] == 0) || (diamondMapPecas[linha_peca_capturada_norte][coluna_peca_capturada_norte] == 1)) {
								qtd_pecas_pretas -= 1;
							}
							else {
								qtd_pecas_brancas -= 1;
							}
							diamondMapPecas[linha_peca_capturada_norte][coluna_peca_capturada_norte] = 5;
							pecaCapturada = true;
						}
						else if ((coluna_peca_destino == coluna_peca_capturar_sul) && (linha_peca_destino == linha_peca_capturar_sul)) {
							if ((diamondMapPecas[linha_peca_capturada_sul][coluna_peca_capturada_sul] == 0) || (diamondMapPecas[linha_peca_capturada_sul][coluna_peca_capturada_sul] == 1)) {
								qtd_pecas_pretas -= 1;
							}
							else {
								qtd_pecas_brancas -= 1;
							}
							diamondMapPecas[linha_peca_capturada_sul][coluna_peca_capturada_sul] = 5;
							pecaCapturada = true;
						}
						else if ((coluna_peca_destino == coluna_peca_capturar_leste) && (linha_peca_destino == linha_peca_capturar_leste)) {
							if ((diamondMapPecas[linha_peca_capturada_leste][coluna_peca_capturada_leste] == 0) || (diamondMapPecas[linha_peca_capturada_leste][coluna_peca_capturada_leste] == 1)) {
								qtd_pecas_pretas -= 1;
							}
							else {
								qtd_pecas_brancas -= 1;
							}
							diamondMapPecas[linha_peca_capturada_leste][coluna_peca_capturada_leste] = 5;
							pecaCapturada = true;
						}
						else if ((coluna_peca_destino == coluna_peca_capturar_oeste) && (linha_peca_destino == linha_peca_capturar_oeste)) {
							if ((diamondMapPecas[linha_peca_capturada_oeste][coluna_peca_capturada_oeste] == 0) || (diamondMapPecas[linha_peca_capturada_oeste][coluna_peca_capturada_oeste] == 1)) {
								qtd_pecas_pretas -= 1;
							}
							else {
								qtd_pecas_brancas -= 1;
							}
							diamondMapPecas[linha_peca_capturada_oeste][coluna_peca_capturada_oeste] = 5;
							pecaCapturada = true;
						}

						//Detectando dama
						if ((coluna_cursor == 0) && (IDPecaClique == 0)) {
							IDPecaClique = 1;
						}
						else if ((coluna_cursor == 7) && (IDPecaClique == 2)) {
							IDPecaClique = 3;
						}

						//Ajustando variáveis após movimento
						diamondMapPecas[linha_peca_origem][coluna_peca_origem] = 5;
						diamondMapPecas[linha_cursor][coluna_cursor] = IDPecaClique;
						coluna_peca_origem = -1;
						linha_peca_origem = -1;
						coluna_peca_destino = -1;
						linha_peca_destino = -1;
						coluna_peca_capturar_norte = -1;
						linha_peca_capturar_norte = -1;
						coluna_peca_capturar_sul = -1;
						linha_peca_capturar_sul = -1;
						coluna_peca_capturar_leste = -1;
						linha_peca_capturar_leste = -1;
						coluna_peca_capturar_oeste = -1;
						linha_peca_capturar_oeste = -1;
						coluna_peca_capturada_norte = -1;
						linha_peca_capturada_norte = -1;
						coluna_peca_capturada_sul = -1;
						linha_peca_capturada_sul = -1;
						coluna_peca_capturada_leste = -1;
						linha_peca_capturada_leste = -1;
						coluna_peca_capturada_oeste = -1;
						linha_peca_capturada_oeste = -1;
						x_caminho = 0.0f;
						y_caminho = 0.0f;
						x_origem = 0.0f;
						y_origem = 0.0f;
						destinoX = false;
						destinoY = false;
						pecaAndando = false;
						x_destino = 0.0f;
						y_destino = 0.0f;

						//Verificar se é possivel capturar outra peça no mesmo turno
						if (pecaCapturada) {
							if (verificarPossivelJogadaMultipla(linha_cursor, coluna_cursor, IDPecaClique)) {
								linha_peca_origem = linha_cursor;
								coluna_peca_origem = coluna_cursor;

								tv->calculoPosicaoDesenho(coluna_peca_origem, linha_peca_origem, tw, th, x_origem, y_origem);
								x_caminho = x_origem;
								y_caminho = y_origem;
							}
							else {
								pecaCapturada = false;
							}
						}

						//Se não capturou peça na jogada, troca o turno
						if (!pecaCapturada) {
							if (vezBrancas) {
								vezBrancas = false;
								vezPretas = true;
							}
							else {
								vezBrancas = true;
								vezPretas = false;
							}
						}
					}

					//Definição de tile a ser utilizado, segundo o tilemap
					IDImagemPecas = diamondMapPecas[i][j];
					colTile = IDImagemPecas % 2;
					rowTile = IDImagemPecas / 2;

					//define coordenadas x e y de mapeamento do tile 
					glUniform1f(
						glGetUniformLocation(shader_programme, "offsetx"), colTile / 2);
					glUniform1f(
						glGetUniformLocation(shader_programme, "offsety"), rowTile / 3);

					//Desenha losango para o tile
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
			glBindVertexArray(0);


			//Define textura Marcadores
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ID_tileMarcador);
			glUniform1i(glGetUniformLocation(shader_programme, "basic_texture"), 0);

			//Define VAO Marcadores
			glBindVertexArray(VAO_Cursor);
			for (int i = 0; i < 8; i++) {	//Linhas
				for (int j = 0; j < 8; j++) {  //Colunas
					tv->calculoPosicaoDesenho(j, i, tw, th, x, y);

					//translação do tile(em x e y baseado em c e r)
					glm::mat4 matrix = glm::mat4(1.0f);
					matrix = glm::translate(matrix, glm::vec3(x, y, -0.1f));
					unsigned int transformLoc = glGetUniformLocation(shader_programme, "matrix");
					glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
					if ((coluna_peca_origem >= 0) && (linha_peca_origem >= 0)) {
						if (diamondMapJogadasValidacao[i][j] == 0) {
							colMarcador = 0 % 2;
							rowMarcador = 0 / 2;
						}
						else {
							colMarcador = 2 % 2;
							rowMarcador = 2 / 2;
						}
					}
					else if (vezBrancas) {
						if ((diamondMapPecas[i][j] == 2) || (diamondMapPecas[i][j] == 3)) {
							colMarcador = 0 % 2;
							rowMarcador = 0 / 2;
						}
						else {
							colMarcador = 2 % 2;
							rowMarcador = 2 / 2;
						}
					}
					else {
						if ((diamondMapPecas[i][j] == 0) || (diamondMapPecas[i][j] == 1)) {
							colMarcador = 0 % 2;
							rowMarcador = 0 / 2;
						}
						else {
							colMarcador = 2 % 2;
							rowMarcador = 2 / 2;
						}
					}

					//define coordenadas x e y de mapeamento de textura 
					glUniform1f(
						glGetUniformLocation(shader_programme, "offsetx"), colMarcador / 2);
					glUniform1f(
						glGetUniformLocation(shader_programme, "offsety"), rowMarcador / 2);

					//Desenha losango para o tile
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
			glBindVertexArray(0);
		}
		

		//Valida clique do mouse e se foi clicado dentro do tabuleiro
		if ((cliqueMouse) && (linha_cursor >= 0) && (linha_cursor <= 7) && (coluna_cursor >= 0) && (coluna_cursor <= 7)){
			//Verifica se a peça clicada é a peça correta do jogador da rodada e se não está ocorrendo uma captura múltipla 
			if ((((vezPretas) && ((diamondMapPecas[linha_cursor][coluna_cursor] == 0) || (diamondMapPecas[linha_cursor][coluna_cursor] == 1))) ||
				((vezBrancas) && ((diamondMapPecas[linha_cursor][coluna_cursor] == 2) || (diamondMapPecas[linha_cursor][coluna_cursor] == 3)))) && (!pecaCapturada)) {
				coluna_peca_origem = coluna_cursor;
				linha_peca_origem = linha_cursor;
				IDPecaClique = diamondMapPecas[linha_peca_origem][coluna_peca_origem];

				tv->calculoPosicaoDesenho(coluna_peca_origem, linha_peca_origem, tw, th, x_origem, y_origem);
				x_caminho = x_origem;
				y_caminho = y_origem;

				//Limpa o mapa das jogadas possíveis
				for (int i = 0; i < 8; i++) {	//Linhas
					for (int j = 0; j < 8; j++) {  //Colunas
						diamondMapJogadasValidacao[i][j] = diamondMapJogadasPadrao[i][j];
					}
				}
				//Valida jogadas possíveis
				identificarJogadasPossiveis(linha_peca_origem, coluna_peca_origem, IDPecaClique);
			}
			//Verifica se o clique corresponde ao destino possível da peça
			else if ((coluna_peca_origem >= 0) && (linha_peca_origem >= 0) && (diamondMapJogadasValidacao[linha_cursor][coluna_cursor] == 0)) {
				coluna_peca_destino = coluna_cursor;
				linha_peca_destino = linha_cursor;
				tv->calculoPosicaoDesenho(coluna_peca_destino, linha_peca_destino, tw, th, x_destino, y_destino);
				pecaAndando = true;
			}
			
			cliqueMouse = false;
		} else {
			cliqueMouse = false;
		}

		//Captura o pressionamento do R para recomeçar
		int stateR = glfwGetKey(window, GLFW_KEY_R);
		if ((stateR == GLFW_PRESS) && (jogoFinalizado)) {
			restart();
		}

		//Captura o pressionamento do ENTER para começar
		int stateStart = glfwGetKey(window, GLFW_KEY_ENTER);
		if ((stateStart == GLFW_PRESS) && (!jogoIniciado)) {
			jogoIniciado = true;
		}

		//Capturar clique do mouse
		static int oldStateClick = GLFW_RELEASE;
		int newStateClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if ((newStateClick == GLFW_RELEASE && oldStateClick == GLFW_PRESS) && (!pecaAndando)) {
			double mx, my;
			cliqueMouse = true;
			glfwGetCursorPos(window, &mx, &my);
			mouse((float)mx, (float)my);
		}
		oldStateClick = newStateClick;

		glfwSwapBuffers(window);
	}

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();
	return 0;
}