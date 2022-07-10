// Bibliotecas utilizadas pelo OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
// Biblioteca com funcoes matematicas
#include <cmath>
#include <math.h>//usar funcoes matematicas
// Bibliotecas padrao
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
// Variaveis Globais
bool click1 = false, click2 = false, click3 = false, parar = false;
char controle = 'b';
double x_1, y_1, x_2, y_2, x_3, y_3;
double pontosPoli[9999][2];
int width = 512, height = 512, cont = 0;//Largura,altura da janela e contador poligono

// Estrututa de dados para o armazenamento dinamico dos pontos da circunferencia
// selecionados pelos algoritmos de rasterizacao
struct pontoCirc
{
	int xC;
	int yC;
	pontoCirc * prox;
};
// Lista encadeada de pontos da circunferencia
// indica o primeiro elemento da lista
pontoCirc * pontosCirc = NULL;
// Funcao para armazenar um ponto na lista
// Armazena como uma Pilha (empilha)
pontoCirc * pushPontoCirc(int x, int y)
{
	pontoCirc * pntC;
	pntC = new pontoCirc;
	pntC->xC = x;
	pntC->yC = y;
	pntC->prox = pontosCirc;
	pontosCirc = pntC;
	return pntC;
}
// Estrututa de dados para o armazenamento dinamico da reta
struct reta
{
	int x1;
	int y1;
	int x2;
	int y2;
	reta * prox;
};
// Lista encadeada de pontos dos extremos das retas
// indica o primeiro elemento da lista
reta * retas = NULL;
// Funcao para armazenar os pontos na lista
// Armazena como uma Pilha (empilha)
reta * pushReta(int x1, int y1, int x2, int y2)
{
	reta * ret;
	ret = new reta;
	ret->x1 = x1;
	ret->y1 = y1;
	ret->x2 = x2;
	ret->y2 = y2;
	ret->prox = retas;
	retas = ret;
	return ret;
}

// Estrututa de dados para o armazenamento dinamico dos pontos
struct ponto
{
	int x;
	int y;
	ponto * prox;
};

ponto * pontos = NULL;
// Funcao para armazenar um ponto na lista
// Armazena como uma Pilha (empilha)
ponto * pushPonto(int x, int y)
{
	ponto * pnt;
	pnt = new ponto;
	pnt->x = x;
	pnt->y = y;
	pnt->prox = pontos;
	pontos = pnt;
	return pnt;
}
// Zera a pilha
ponto * popPonto()
{
	ponto * pnt;
	pnt = NULL;
	while(pontos != NULL)
	{
		pnt = pontos->prox;
		delete pontos;
		pontos = pnt;
	}
	return pnt;
}

// Declaracoes forward das funcoes utilizadas
void init(void);
void reshape(int, int );
void display(void);
void keyboard(unsigned char , int , int );
void mouse(int , int , int , int );
// Funcao que implementa o Algoritmo Imediato para rasterizacao de retas/ bresenham completo
void retaImediata(double , double , double , double );
// Funcao que percorre a lista de pontos desenhando-os na tela
void drawPontos();
// Funcoes da tarefa
void bresenhamPOSom(double , double , double , double );
void bresenhamCircunferencia(double, double, double);
void desenharQuadrilateros(double , double , double , double );
void desenharTriangulo(double , double , double , double , double , double );
void desenharPoligono(double [][2], int );
void translacao();
void reflexao();
void escala();
void cisalhamento();
void rotacao();
void desenharCircunferencia(double, double, double);
void floodFill(GLint , GLint , float* , float* );
// Funcao Principal do C
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Passagens de parametro C para o glut
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
	glutInitWindowSize (width, height);  // Tamanho da janela do OpenGL
	glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
	glutCreateWindow ("Rasterizacao"); // Da nome para uma janela OpenGL
	init(); // Chama funcao init();
	glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
	glutKeyboardFunc(keyboard); //funcao callback do teclado
	glutMouseFunc(mouse); //funcao callback do mouse
	glutDisplayFunc(display); //funcao callback de desenho
	glutMainLoop(); // executa o loop do OpenGL
	return 0; // retorna 0 para o tipo inteiro da funcao main();
}

// Funcao com alguns comandos para a inicializacao do OpenGL;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}

void reshape(int w, int h)
{
	// Reinicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	glOrtho (0, w, 0, h, -1 , 1);
	// muda para o modo GL_MODELVIEW (não pretendemos alterar a projecção
	// quando estivermos a desenhar na tela)
	glMatrixMode(GL_MODELVIEW);
}

// Funcao usada na funcao callback para utilizacao das teclas normais do teclado
void keyboard(unsigned char key, int x, int y)
{
	switch (key)   // key - variavel que possui valor ASCII da tecla precionada
	{
	case 27: // codigo ASCII da tecla ESC
		exit(0); // comando pra finalizacao do programa
		break;
	case 'a': // letra a
		controle = 'a';
		break;
	case 'b': // letra b
		controle = 'b';
		break;
	case 'c': // letra c
		controle = 'c';
		break;
	case 'd': // letra d
		controle = 'd';
		break;
	case 'e': // letra e
		controle = 'e';
		break;
	case 'f': // letra f translacao
		controle = 'f';
		glutPostRedisplay();
		break;
	case 'g': // letra f escala
		controle = 'g';
		glutPostRedisplay();
		break;
	case 'h': // letra f cisalhamento
		controle = 'h';
		glutPostRedisplay();
		break;
	case 'i': // letra f reflexao
		controle = 'i';
		glutPostRedisplay();
		break;
	case 'j': // letra f rotacao
		controle = 'j';
		glutPostRedisplay();
		break;
	case 'k': // letra g desenhar circunferencia
		controle = 'k';
		glutPostRedisplay();
		break;
	case 'm': // letra i Flood Fill
		controle = 'm';
		break;
	case 'z': // para parar de ler o poligono
		controle = 'z';
		parar = true;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// Funcao usada na funcao callback para a utilizacao do mouse
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{

			if(controle == 'm')
			{
				if(!click1)
				{
					click1 = true;
					x_1 = x;
					y_1 = height - y;
					printf("x1y1(%.0f,%.0f)\n", x_1, y_1);
					glutPostRedisplay();
				}
			}
			else
			{
				if(controle == 'k')
				{
					if(!click1)
					{
						click1 = true;
						x_1 = x;
						y_1 = height - y;
						printf("x1y1(%.0f,%.0f)\n", x_1, y_1);
						glutPostRedisplay();
					}
				}
				else if(controle == 'e')
				{
					if(!parar)
					{
						pontosPoli[cont][0] = x;
						pontosPoli[cont][1] = height - y;
						printf("x%dy%d(%.0f,%.0f)\n", cont, cont, pontosPoli[cont][0], pontosPoli[cont][1]);
						cont++;
					}
				}
				else
				{
					if(controle == 'z')
					{
						cont = 0 ;
						glutPostRedisplay();
					}
					else
					{
						if(controle == 'd')
						{
							if(click1 && click2)
							{
								click3 = true;
								x_3 = x;
								y_3 = height - y;
								printf("x3y3(%.0f,%.0f)\n", x_3, y_3);
								glutPostRedisplay();
							}
							else
							{
								if(click1)
								{
									click2 = true;
									x_2 = x;
									y_2 = height - y;
									printf("x2y2(%.0f,%.0f)\n", x_2, y_2);
								}
								else
								{
									click1 = true;
									x_1 = x;
									y_1 = height - y;
									printf("x1y1(%.0f,%.0f)\n", x_1, y_1);
								}
							}
						}
						else
						{
							if(click1)
							{
								click2 = true;
								x_2 = x;
								y_2 = height - y;
								printf("x2y2(%.0f,%.0f)\n", x_2, y_2);
								glutPostRedisplay();
							}
							else
							{
								click1 = true;
								x_1 = x;
								y_1 = height - y;
								printf("x1y1(%.0f,%.0f)\n", x_1, y_1);
							}
						}
					}
				}
			}
		}

		break;
	default:
		break;
	}
}

// Funcao usada na funcao callback para desenhar na tela
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Limpa o buffer de cores e reinicia a matriz
	glLoadIdentity();
	glColor3f (0.0, 0.0, 0.0); // Seleciona a cor default como preto
	switch(controle)
	{
	case 'a':
		if(click1 && click2)
		{
			bresenhamPOSom(x_1, y_1, x_2, y_2);
			drawPontos();
			click1 = false;
			click2 = false;
		}
		break;
	case 'b':
		drawPontos();
		if(click1 && click2)
		{
			retaImediata(x_1, y_1, x_2, y_2);
			drawPontos();
			click1 = false;
			click2 = false;
		}
		break;
	case 'c':
		if(click1 && click2)
		{
			desenharQuadrilateros(x_1, y_1, x_2, y_2);
			drawPontos();
			click1 = false;
			click2 = false;
		}
		break;
	case 'd':
		if(click1 && click2 && click3)
		{
			desenharTriangulo(x_1, y_1, x_2, y_2, x_3, y_3);
			drawPontos();
			click1 = false;
			click2 = false;
			click3 = false;
		}
		break;
	case 'e':
		drawPontos();
		break;
	case 'f':
		translacao();
		controle = 'b';
		drawPontos();
		break;
	case 'g':
		escala();
		controle = 'b';
		drawPontos();
		break;
	case 'h':
		cisalhamento();
		controle = 'b';
		drawPontos();
		break;
	case 'i':
		reflexao();
		controle = 'b';
		drawPontos();
		break;
	case 'j':
		rotacao();
		controle = 'b';
		drawPontos();
		break;
	case 'k':
		if(click1)
		{
			double raio;
			printf("Digite o raio da circunferencia: ");
			scanf("%lf", &raio);
			desenharCircunferencia(x_1, y_1, raio);
			click1 = false;
			controle = 'b';
		}
		drawPontos();
		break;
	case 'm':
		if(click1)
		{
			float old[] = {0, 0, 0};
			glPointSize(1);
			float newC[3] = {0, 1, 1};
			floodFill(x_1, y_1, old, newC );
			click1 = false;
			controle = 'b';
		}
		break;
	case 'z':
		desenharPoligono(pontosPoli, cont);
		controle = 'b';
		drawPontos();
		break;
	default:
		break;
	}
	glutSwapBuffers(); // manda o OpenGl renderizar as primitivas
}

//Funcao que desenha os pontos contidos em uma lista de pontos
void drawPontos()
{
	ponto * pnt;
	pnt = pontos;
	pontoCirc * pntC;
	pntC = pontosCirc;
	glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
	// Desenha os pontos
	while(pnt != NULL)
	{
		glVertex2i(pnt->x, pnt->y);
		pnt = pnt->prox;
	}
	// Desenha pontos da circunferencia
	while(pntC != NULL)
	{
		glVertex2i(pntC->xC, pntC->yC);
		pntC = pntC->prox;
	}
	glEnd();  // indica o fim do desenho
}

// Bresenham primeiro octante somente
void bresenhamPOSom(double x1, double y1, double x2, double y2)
{
	int dx, dy, d, incE, incNE, x, y, xf;

	if (x1 > x2)
	{
		x = x2;
		y = y2;
		xf = x1;
		dx = x1 - x2;
		dy = y1 - y2;
	}
	else
	{
		x = x1;
		y = y1;
		xf = x2;
		dx = x2 - x1;
		dy = y2 - y1;
	}
	d = 2 * dy - dx;
	incE = 2 * dy;
	incNE = 2 * (dy - dx);
	pushPonto(x, y);
	while (x < xf)
	{
		x++;
		if (d <= 0)
			d += incE;
		else
		{
			y++;
			d += incNE;
		}
		pushPonto(x, y);
	}

}

// Bresenham reducao primeiro octante totalmente funcional
void retaImediata(double x1, double y1, double x2, double y2)
{
	pushReta(x1, y1, x2, y2);
	int dx, dy, aux1, aux2, aux_d, aux_x, aux_y, x, y, xf, d, incE, incNE;
	int declive, simetrico;
	dx = x2 - x1;
	dy = y2 - y1;
	declive = 0;
	simetrico = 0;

	if (dx * dy < 0)
	{
		y1 = y1 * (-1);
		y2 = y2 * (-1);
		dy = dy * (-1);
		simetrico = 1;
	}
	if (abs(dx) < abs(dy))
	{
		aux1 = x1;
		aux2 = x2;
		x1 = y1;
		x2 = y2;
		y1 = aux1;
		y2 = aux2;
		aux_d = dx;
		dx = dy;
		dy = aux_d;
		declive = 1;
	}
	if (x1 > x2)
	{
		aux_x = x1;
		x1 = x2;
		x2 = aux_x;
		aux_y = y1;
		y1 = y2;
		y2 = aux_y;
		dx = dx * (-1);
		dy = dy * (-1);
	}

	if (x1 > x2)
	{
		x = x2;
		y = y2;
		xf = x1;
		dx = x1 - x2;
		dy = y1 - y2;
	}
	else
	{
		x = x1;
		y = y1;
		xf = x2;
		dx = x2 - x1;
		dy = y2 - y1;
	}

	d = 2 * dy - dx;
	incE = 2 * dy;
	incNE = 2 * (dy - dx);
	int i;
	for(i = (int)x; i < (int)xf; i++)
	{
		if (d <= 0)
		{
			x++;
			d += incE;

		}
		else
		{
			x++;
			y++;
			d += incNE;
		}
		if((declive == 1) && (simetrico == 1))
		{
			pushPonto(y, -x);
		}

		else if(simetrico == 1)
		{
			pushPonto(x, -y);
		}
		else if(declive == 1)
		{

			pushPonto(y, x);
		}
		else
		{
			pushPonto(x, y);
		}

	}
	drawPontos();
}

// Desenha um quadrilatero usando o ponto da esquerda superior e direita inferior
void desenharQuadrilateros(double x1, double y1, double x2, double y2)
{
	retaImediata(x1, y1, x2, y1);
	retaImediata(x2, y1, x2, y2);
	retaImediata(x2, y2, x1, y2);
	retaImediata(x1, y2, x1, y1);
	drawPontos();
}

// Desenha um triangulo a cada 3 pontos dados
void desenharTriangulo(double x1, double y1, double x2, double y2, double x3, double y3)
{
	retaImediata(x1, y1, x2, y2);
	retaImediata(x2, y2, x3, y3);
	retaImediata(x3, y3, x1, y1);
	drawPontos();
}

// Desenha um poligono, ativando a funcao voce clica na tela e apos clicar em 'z' o poligono e feito
void desenharPoligono(double pontos[][2], int quantidadeDePontos)
{
	int i;
	for(i = 0; i < quantidadeDePontos - 1; i++)
	{
		retaImediata(pontos[i][0], pontos[i][1], pontos[i + 1][0], pontos[i + 1][1]);
	}
	retaImediata(pontos[i][0], pontos[i][1], pontos[0][0], pontos[0][1]);
	parar = false;
	cont = 0;
	drawPontos();
}

// Translada todos os ponto que estao na tela, voce ativa a funcao digita o fator X e Y pelo console e ao terminar a execucao volta a desenhar retas
void translacao()
{
	double FatorXtranslacao, FatorYtranslacao;
	printf("Digite o fator X: ");
	scanf("%lf", &FatorXtranslacao);
	printf("Digite o fator Y: ");
	scanf("%lf", &FatorYtranslacao);
	ponto * pnt;
	pnt = pontos;
	while(pnt != NULL)
	{
		pnt->x = pnt->x + FatorXtranslacao;
		pnt->y = pnt->y + FatorYtranslacao;
		pnt = pnt->prox;
	}
	reta * pnt2;
	pnt2 = retas;
	while(pnt2 != NULL)
	{
		pnt2->x1 = pnt2->x1 + FatorXtranslacao;
		pnt2->y1 = pnt2->y1 + FatorYtranslacao;
		pnt2->x2 = pnt2->x2 + FatorXtranslacao;
		pnt2->y2 = pnt2->y2 + FatorYtranslacao;
		pnt2 = pnt2->prox;
	}
	pontoCirc * pnt3;
	pnt3 = pontosCirc;
	while(pnt3 != NULL)
	{
		pnt3->xC = pnt3->xC + FatorXtranslacao;
		pnt3->yC = pnt3->yC + FatorYtranslacao;
		pnt3 = pnt3->prox;
	}
	drawPontos();
}

// faz a reflexao de todos os pontos que estao na tela, voce ativa a funcao digita o eixo X ou Y pelo console e ao terminar a execucao volta a desenhar retas
void reflexao()
{
	char eixo;
	printf("Digite o eixo: ");
	scanf("%c", &eixo);
	ponto * pnt;
	pnt = pontos;
	while(pnt != NULL)
	{
		if(eixo == 'x')
		{
			pnt->x = pnt->x ;
			pnt->y = pnt->y * (-1);
			pnt = pnt->prox;
		}
		else if(eixo == 'y')
		{
			pnt->x = pnt->x * (-1);
			pnt->y = pnt->y ;
			pnt = pnt->prox;
		}
		else
		{
			printf("digitado errado");
		}
	}
	reta * pnt2;
	pnt2 = retas;
	while(pnt2 != NULL)
	{
		if(eixo == 'x')
		{
			pnt2->x1 = pnt2->y1 * (-1);
			pnt2->x2 = pnt2->y2 * (-1);
			pnt2 = pnt2->prox;
		}
		else if(eixo == 'y')
		{
			pnt2->y1 = pnt2->x1 * (-1);
			pnt2->y2 = pnt2->x2 * (-1);
			pnt2 = pnt2->prox;
		}
		else
		{
			printf("digitado errado");
		}
	}
	pontoCirc * pnt3;
	pnt3 = pontosCirc;
	while(pnt3 != NULL)
	{
		if(eixo == 'x')
		{
			pnt3->xC = pnt3->xC ;
			pnt3->yC = pnt3->yC * (-1);
			pnt3 = pnt3->prox;
		}
		else if(eixo == 'y')
		{
			pnt3->xC = pnt3->xC * (-1);
			pnt3->yC = pnt3->yC ;
			pnt3 = pnt3->prox;
		}
		else
		{
			printf("digitado errado");
		}
	}
	drawPontos();
}

// faz a escala das retas, quadrilateros, triagulos e poligonos desenhados na tela, voce digita o fator X e Y da escala e ao terminar ele volta a desenhar retas
void escala()
{

	double fatorXEscala, fatorYEscala;
	printf("Digite o fator X: ");
	scanf("%lf", &fatorXEscala);
	printf("Digite o fator Y: ");
	scanf("%lf", &fatorYEscala);
	popPonto();
	reta * ret;
	ret = retas;
	while(ret != NULL)
	{
		ret->x1 = ret->x1 * fatorXEscala;
		ret->y1 = ret->y1 * fatorYEscala;
		ret->x2 = ret->x2 * fatorXEscala;
		ret->y2 = ret->y2 * fatorYEscala;
		retaImediata(ret->x1, ret->y1, ret->x2, ret->y2);
		ret = ret->prox;
	}

	drawPontos();
}

// Faz o cisalhamento de qualquer reta, quadilatero, triangulos e poligonos desenhados na tela.
void cisalhamento()
{
	char eixo;
	double fatorCisalhamento;
	printf("Digite o eixo: ");
	scanf("%c", &eixo);
	printf("Digite o fator: ");
	scanf("%lf", &fatorCisalhamento);
	popPonto();
	reta * ret;
	ret = retas;
	while(ret != NULL)
	{
		if(eixo == 'x')
		{
			ret->x1 = ret->x1 + (fatorCisalhamento * ret->y1);
			ret->x2 = ret->x2 + (fatorCisalhamento * ret->y2);
			retaImediata(ret->x1, ret->y1, ret->x2, ret->y2);
			ret = ret->prox;
		}
		else if(eixo == 'y')
		{
			ret->y1 = ret->y1 + (fatorCisalhamento * ret->x1);
			ret->y2 = ret->y2 + (fatorCisalhamento * ret->x2);
			retaImediata(ret->x1, ret->y1, ret->x2, ret->y2);
			ret = ret->prox;
		}
		else
		{
			printf("digitado errado");
		}
	}
	drawPontos();
}

// Faz a rotacao de todos as retas, quadrilateros, triagulos e poligonos desenhados na tela, entrada em graus pelo console
void rotacao()
{

	double graus, rad;
	printf("Digite os graus: ");
	scanf("%lf", &graus);
	rad = (graus * 3.14) / 180;
	ponto * pnt;
	pnt = pontos;
	while(pnt != NULL)
	{
		pnt->x = (pnt->x * cos(rad)) - (sin(rad) * pnt->y);
		pnt->y = (pnt->x * sin(rad)) + (cos(rad) * pnt->y);
		pnt = pnt->prox;
	}
	reta * ret;
	ret = retas;
	while(ret != NULL)
	{
		ret->x1 = (ret->x1 * cos(rad)) - (sin(rad) * ret->y1);
		ret->y1 = (ret->x1 * sin(rad)) + (cos(rad) * ret->y1);
		ret->x2 = (ret->x2 * cos(rad)) - (sin(rad) * ret->y2);
		ret->y2 = (ret->x2 * sin(rad)) + (cos(rad) * ret->y2);
		ret = ret->prox;
	}
	drawPontos();
}

// Desenha um circunferencia apos um click na tela voce da a entrada pelo console do raio;
void desenharCircunferencia(double x1, double y1, double raio)
{
	double x, y, d, E, SE;

	d = 1 - raio;
	E = 3;
	SE = (-2 * raio) + 5;
	x = 0;
	y = raio;
	if(x1 != 0 && y1 != 0)
	{
		pushPontoCirc(x1, y1 + raio);
		pushPontoCirc(x1, y1 - raio);
		pushPontoCirc(x1 + raio, y1);
		pushPontoCirc(x1 - raio, y1);
		while(x < y)
		{
			if (d < 0)
			{
				d = d + E;
				E = E + 2;
				SE = SE + 2;
			}
			else
			{
				d = d + SE;
				E = E + 2;
				SE = SE + 4;
				y--;
			}

			x++;
			pushPontoCirc(x + x1, y + y1);
			pushPontoCirc(y + x1,  x + y1);
			pushPontoCirc(y + x1,  - x + y1);
			pushPontoCirc(x + x1,  - y + y1);
			pushPontoCirc( - x + x1,  - y + y1);
			pushPontoCirc( - y + x1,  - x + y1);
			pushPontoCirc( - y + x1,  x + y1);
			pushPontoCirc( - x + x1,  y + y1);
		}
	}
	drawPontos();
}



// Faz o preenchimento de um poligono qualquer, dado um click em qualquer parte desse poligono
void floodFill(GLint x, GLint y, float * oldColor, float * newColor)
{
	float color[3];
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);
	drawPontos();
	if((color[0] != newColor[0] || color[1] != newColor[1] || color[2] != newColor[2]) )
	{
		glColor3f(newColor[0], newColor[1], newColor[2]);
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		pushPonto(x, y);
		glutSwapBuffers();
		floodFill(x + 1, y, oldColor, newColor);
		floodFill(x - 1, y, oldColor, newColor);
		floodFill(x, y + 1, oldColor, newColor);
		floodFill(x, y - 1, oldColor, newColor);

	}
	drawPontos();
}
//TUTORIAL
//- CADA LETRA CORRESPONDE A UMA FUNCAO DO PROGRAMA
//- ELAS PRECISAM SER DIGITADAS MINUSCULA
//- PRESTAR ATENCAO POIS ALGUMAS FUNCOES NECESSITAM DE ENTRADA DIGITADA ATRAVES DO CONSOLE//ELAS SAO INDICADA PELA LETRA C
//a - desenha linha de bresenham somente para o primeiro octante
//b - desenha linha de bresenham com reducao para o primeiro octante
//c - desenha um quadrilatero a cada dois clicks
//d - desenha um triangulo a cada tres clicks
//e - desenha um poligono, dar quantos clicks na tela quiser e ao final teclar z para que o poligono seja construido
//f - faz a translacao do objeto - C
//g - faz a escala do objeto - C
//h - faz o cisalhamento - C
//i - faz a reflexao - C
//j - faz a rotacao - C
//k - desenha uma circunferencia dado um click mostrando o centro e o raio precisa ser digitado no console - C
//m - um click e o poligono e preenchido por completo, tomar cuidado com poligonos grandes vai acontecer muitas recursoes
