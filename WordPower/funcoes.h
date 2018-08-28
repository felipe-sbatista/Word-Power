#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "tipos.h"

void instrucao();
void cadastro_de_palavras(FILE*arquivo_palavras);
void seta_quadrados(ALLEGRO_BITMAP* quadrados[], int tamanho);
bool modo3();
bool modo4();
bool modo5();
bool modo6();
void colorir(ALLEGRO_BITMAP*quadrados[], int tamanho);
bool inicializa();
void finalizar();
void manipular_entrada(ALLEGRO_EVENT evento, char[]);
void Errou(Tpalavra palavra_atual, int pontuacao);
void preenche_coordenada(int matriz[][2], int tamanho);
int busca_posicao(int matriz[][2], int quadrado, int tamanho);
void exibir_texto_centralizado(ALLEGRO_FONT *fonte);
void embaralha(char word[]);
void registrar_usuario(Tusuario Usuario);
FILE * criarArquivo(char nome[]);
void exibir_recordes();
Tpalavra getPalavra(Tpalavra dicionario[]);
void toVetor(Tpalavra dicionario[], FILE*arq_palavras);
void InitBG(Tback *back, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem);
void atualizarBG(Tback *back);
void printaBG(Tback *back);
void explicacao(Tpalavra palavra_atual, ALLEGRO_FONT *fonte);
TAudio getAudio();
bool checkPoint();
void printa_menu(ALLEGRO_FONT *fonte, bool sair, bool recordes, bool iniciar);
void printa_seta(ALLEGRO_BITMAP *seta, int x, int y);
void exibe_legendas(int pontuacao, int vidas, ALLEGRO_FONT *fonte);
void inicializa_palavras_do_checkPoint();
bool inicializa_audios();
bool inicializa_imagens();
void som_clicar();
void palavra_correta(ALLEGRO_FONT *fonte, char palavra[]);
void menu_jogo(ALLEGRO_FONT *fonte);
void errou_checkpoint(char palavra[]);
