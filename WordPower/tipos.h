#include "allegro5/allegro.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"

#ifndef ESTRUTURAS
#define ESTRUTURAS
typedef struct Tpalavra {
    char traducao[15];
    char palavra[8];
    char descricao[35];
    int status;
}Tpalavra;
typedef struct Tusuario {
    char nome_usuario[51];
    int recorde, vidas;
}Tusuario;
typedef struct Tback {
    float x, y;
    float velocidade_x, velocidade_y;
    int dirX, dirY;
    int largura, altura;
    ALLEGRO_BITMAP *backGround;
}Tback;
typedef struct TAudio {
    char palavra[10];
    ALLEGRO_SAMPLE * audio;
}TAudio;
#endif

