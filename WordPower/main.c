
// Os arquivos de cabeçalho
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tipos.h"
#include "funcoes.h"

// Atributos da tela
const int PALAVRAS_SALVAS = 5;
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
int pontuacao;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila = NULL, *fila_contador = NULL;
ALLEGRO_SAMPLE *som_acertou = NULL, *som_errou = NULL, *som_click = NULL;
ALLEGRO_AUDIO_STREAM *som_fundo = NULL;
ALLEGRO_BITMAP *imagem_fundo = NULL, *imagem_coracao = NULL, *imagem_podio = NULL, *seta_direita = NULL, *seta_esquerda = NULL, *botao_play = NULL;
FILE *arq_usuarios, *arq_palavras_tamanho3, *arq_palavras_tamanho4,*arq_palavras_tamanho5,*arq_palavras_tamanho6;
char nome[51];
TAudio lista_audio[10];



int main(void) {
    arq_usuarios = criarArquivo("Usuarios.txt");
    arq_palavras_tamanho4 = criarArquivo("Palavras4.txt");
    arq_palavras_tamanho3 = criarArquivo("Palavras3.txt");
    arq_palavras_tamanho5 = criarArquivo("Palavras5.txt");
    arq_palavras_tamanho6 = criarArquivo("Palavras6.txt");
    bool iniciar = false, sair = false, recordes = false;
    ALLEGRO_FONT *fonte = NULL;
    Tback BG;
//    cadastro_de_palavras(arq_palavras_tamanho3);
//    printf("bora pra 4\n");
//    cadastro_de_palavras(arq_palavras_tamanho4);
//    printf("para 5\n");
//    cadastro_de_palavras(arq_palavras_tamanho5);
//    printf("bora pra 6\n");
//  // cadastro_de_palavras(arq_palavras_tamanho6);

    if (!inicializa()) { return -1; }
    inicializa_audios();
    inicializa_imagens();
    //inicializa o background
    InitBG(&BG, 0, 0, 0.01, 0, LARGURA_TELA, ALTURA_TELA, -1, 1, imagem_fundo);

    //cadastro_de_palavras();
    al_attach_audio_stream_to_mixer(som_fundo, al_get_default_mixer());


    // Carregando o arquivo de fonte
    fonte = al_load_font("Orange.ttf", 50, 0);
    if (!fonte) { al_destroy_display(janela);	fprintf(stderr, "Falha ao carregar fonte.\n");	return -1; }

    al_register_event_source(fila, al_get_mouse_event_source());
    al_set_audio_stream_playmode(som_fundo, ALLEGRO_PLAYMODE_LOOP);
    while (!sair) {
        iniciar = false;
        recordes = false;
        while (!al_is_event_queue_empty(fila)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);
            atualizarBG(&BG);
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                atualizarBG(&BG);
                if (evento.mouse.x >= LARGURA_TELA / 2 - 50 && evento.mouse.x <= LARGURA_TELA / 2 + 50 && evento.mouse.y >= ALTURA_TELA / 3 + 10 - 50 && evento.mouse.y <= ALTURA_TELA / 3 + 10 + 50) {
                    iniciar = true;
                }
                    //sair
                else if (evento.mouse.x >= LARGURA_TELA / 2 - 50 && evento.mouse.x <= LARGURA_TELA / 2 + 50 && evento.mouse.y >= ALTURA_TELA / 2 + 180 - 50 && evento.mouse.y <= ALTURA_TELA / 2 + 180 + 50) {
                    sair = true;
                }
                    //recordes ta bugado ainda, deu erro de leitura
                else if (evento.mouse.x >= LARGURA_TELA / 2 - 50 && evento.mouse.x <= LARGURA_TELA / 2 + 50 && evento.mouse.y >= ALTURA_TELA / 2 && evento.mouse.y <= ALTURA_TELA / 2 + 45 + 50) {
                    recordes = true;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = true;
            }
        }
        printaBG(&BG);
        printa_menu(fonte, sair, recordes, iniciar);
        atualizarBG(&BG);
    }
    // Desalocacao da fonte e da janela
    finalizar();
    return 0;
}


//Fun��es principais
void exibir_recordes() {
    Tusuario usuario;
    int status, x = 70, y = 50;
    ALLEGRO_FONT *fonte;
    fonte = al_load_font("queen.ttf", 25, 0);
    if (!fonte) {
        al_destroy_display(janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return;
    }
    al_clear_to_color(al_map_rgb(255, 255, 200));
    fseek(arq_usuarios, 0, 0);
    while (1) {
        status = fread(&usuario, sizeof(Tusuario), 1, arq_usuarios);
        if (status != 1) {
            if (feof(arq_usuarios)) {
                printf("Fim de arquivo\n");
                break;
            }
            else {
                fprintf(stderr, "Erro na leitura\n");
                return;
            }
        }
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), x, y, 0, "%s - %d", usuario.nome_usuario, usuario.recorde);
        y += 30;
        if (y >= ALTURA_TELA) {
            x = 140; y = 50;
        }
        al_flip_display();
    }
    printa_seta(seta_esquerda, 10, 10);
}

void instrucao() {
    ALLEGRO_FONT *fonte = NULL;
    fonte = al_load_font("queen.ttf", 27, 0);
    al_clear_to_color(al_map_rgb(255, 255, 200));
    if (!fonte) { al_destroy_display(janela); fprintf(stderr, "Falha ao carregar fonte.\n"); return; }
    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 200, ALLEGRO_ALIGN_CENTER, "INSTRUCOES");
    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 150, ALLEGRO_ALIGN_CENTER, "-Uma palavra por rodada sera sorteada para voce e voce tera que selecionar ");
    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 120, ALLEGRO_ALIGN_CENTER, "a ordem correta das letras para formar a palavra em ingles.Voce inicia com 3 ");
    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 90, ALLEGRO_ALIGN_CENTER, "letras e ao decorrer do jogo, ira aumentando ate 6 letras ");
    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 60, ALLEGRO_ALIGN_CENTER, "A cada nivel o tamanho das palavras vao aumentando e voce chegara ao checkpoint ");
    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 30, ALLEGRO_ALIGN_CENTER, "onde voce deve digitar a palavra referente ao audio para seguir em frente!");
    al_flip_display();
    al_rest(1.0);
}

bool modo6(){
    ALLEGRO_BITMAP *quadrados[6];
    ALLEGRO_FONT *fonte;
    Tusuario Usuario; Usuario.vidas = 0;
    Tpalavra dicionario[6], palavra_atual;
    int tamanho = 6;
    int i, qtd = 0, coordenadas[6][2], posicao = 0;
    bool  quadrado1 = false, quadrado2 = false, quadrado3 = false, quadrado4 = false,quadrado5 = false, quadrado6=false;
    char auxiliar[8], palavra[8];
    float larg1 = 200, alt1 = 130, larg2 = 400, alt2 = 280, alt3=370;

    //para setar os quadrados depois
    preenche_coordenada(coordenadas, tamanho);//zera o vetor com as posicoes posteriores dos quadrados

    //passa todas as palavras do arquivo para o dicionario
    toVetor(dicionario, arq_palavras_tamanho6);
    strcpy(nome, "");
    fonte = al_load_font("queen.ttf", 35, 0);
    if (!fonte) { al_destroy_display(janela); fprintf(stderr, "Falha ao carregar fonte.\n"); return false; }

    //cria os bitmaps dos quadrados
    seta_quadrados(quadrados, tamanho);

    //colore os quadrados
    colorir(quadrados, tamanho);

    // volta para janela
    al_set_target_bitmap(al_get_backbuffer(janela));

    al_flip_display();
    //loop de palavras
    for (i = 0; i < 2; i++) {
        qtd = 0;
        quadrado1 = false;
        quadrado2 = false;
        quadrado3 = false;
        quadrado4 = false;
        quadrado5 = false;
        quadrado6 = false;
        //inicializar fila
        fila = al_create_event_queue();
        if (!fila) {
            fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
            al_destroy_display(janela);
            return false;
        }
        al_register_event_source(fila, al_get_mouse_event_source());

        //retira uma palavra aleatoria
        palavra_atual = getPalavra(dicionario);
        strcpy(auxiliar, palavra_atual.palavra);

        //faz o shuffle na palavra
        embaralha(auxiliar);
        puts(palavra_atual.palavra);
        puts(auxiliar);

        //loop de jogabilidade
        while (qtd < tamanho) {
            while (!al_is_event_queue_empty(fila)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila, &evento);
                //se houve um click
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    //primeiro quadrado
                    if (quadrado1 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[0]) &&
                        evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[0])) {
                        palavra[qtd] = auxiliar[1];
                        quadrado1 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 1;
                        som_clicar();
                    }
                        //segundo quadrado a direita
                    else if (quadrado2 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[1]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[1])) {
                        palavra[qtd] = auxiliar[3];
                        quadrado2 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 2;
                        som_clicar();
                    }
                        //terceiro quadrado
                    else if (quadrado3 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[2]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[2]) &&
                             evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[2]) &&
                             evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[2])) {
                        palavra[qtd] = auxiliar[0];
                        quadrado3 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 3;
                        som_clicar();
                    }
                        //quarto quadrado
                    else if (quadrado4 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[3]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[3]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[3]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[3])) {
                        palavra[qtd] = auxiliar[2];
                        quadrado4 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 4;
                    }
                    else if (quadrado5 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[4]) &&
                             evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[4]) &&
                             evento.mouse.y >= alt3 - al_get_bitmap_height(quadrados[4]) &&
                             evento.mouse.y <= alt3 + al_get_bitmap_height(quadrados[4])) {
                        palavra[qtd] = auxiliar[4];
                        quadrado5 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 5;
                    }
                    else if (quadrado6 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[5]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[5]) &&
                             evento.mouse.y >= alt3 - al_get_bitmap_height(quadrados[5]) &&
                             evento.mouse.y <= alt3 + al_get_bitmap_height(quadrados[5])) {
                        palavra[qtd] = auxiliar[5];
                        quadrado6 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 6;
                    }
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return false;
                }
            }

            al_clear_to_color(al_map_rgb(255, 255, 200));
            ////define a cor para os quadrados
            //colorir(um, dois, tres, quatro);
            //volta para janela
            al_set_target_bitmap(al_get_backbuffer(janela));
            //Printa a pontuacao e a quantidade de vida
            exibe_legendas(pontuacao, Usuario.vidas, fonte);

            //printa as letras e caso ja tenha sido selecionadas, andam com o quadrado

            if (quadrado1 == false) {
                al_draw_bitmap(quadrados[0], larg1, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[1]);
            } else {
                posicao = busca_posicao(coordenadas, 1, tamanho);
                al_draw_bitmap(quadrados[0], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[1]);
                al_rest(0.2);
            }

            if (quadrado2 == false) {
                al_draw_bitmap(quadrados[1], larg1, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[3]);
            } else {
                posicao = busca_posicao(coordenadas, 2, tamanho);
                al_draw_bitmap(quadrados[1], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[3]);
                al_rest(0.2);
            }

            if (quadrado3 == false) {
                al_draw_bitmap(quadrados[2], larg2, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[0]);
            } else {
                posicao = busca_posicao(coordenadas, 3, tamanho);
                al_draw_bitmap(quadrados[2], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[0]);
                al_rest(0.2);
            }

            if (quadrado4 == false) {
                al_draw_bitmap(quadrados[3], larg2, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[2]);
            } else {
                posicao = busca_posicao(coordenadas, 4, tamanho);
                al_draw_bitmap(quadrados[3], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[2]);
                al_rest(0.2);
            }

            if (quadrado5 == false) {
                al_draw_bitmap(quadrados[4], larg1, alt3, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt3 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[4]);
            } else {
                posicao = busca_posicao(coordenadas, 5, tamanho);
                al_draw_bitmap(quadrados[4], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[4]);
                al_rest(0.2);
            }

            if (quadrado6 == false) {
                al_draw_bitmap(quadrados[5], larg2, alt3, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt3 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[5]);
            } else {
                posicao = busca_posicao(coordenadas, 6, tamanho);
                al_draw_bitmap(quadrados[5], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[5]);
                al_rest(0.2);
            }
            al_flip_display();
        }

        //atualizao display com as letras
        al_flip_display();

        //verifica se o usuario escolheu a ordem correta
        if (strcmp(palavra, palavra_atual.palavra) == 0) {
            palavra_correta(fonte, palavra);
            pontuacao++;
        }
            //palavra errada
        else {
            Errou(palavra_atual, pontuacao);
            Usuario.recorde = pontuacao;
            registrar_usuario(Usuario);
            return false;
        }
    }
    return true;
}

bool modo5(){
    ALLEGRO_BITMAP *quadrados[5];
    ALLEGRO_FONT *fonte;
    Tusuario Usuario; Usuario.vidas = 0;
    Tpalavra dicionario[6], palavra_atual;
    int tamanho = 5;
    int i, qtd = 0, coordenadas[5][2], posicao = 0;
    bool  quadrado1 = false, quadrado2 = false, quadrado3 = false, quadrado4 = false,quadrado5 = false;
    char auxiliar[8], palavra[8];
    float larg1 = 200, alt1 = 130, larg2 = 400, alt2 = 280;

    //para setar os quadrados depois
    preenche_coordenada(coordenadas, tamanho);//zera o vetor com as posicoes posteriores dos quadrados

    //passa todas as palavras do arquivo para o dicionario
    toVetor(dicionario, arq_palavras_tamanho5);
    strcpy(nome, "");
    fonte = al_load_font("queen.ttf", 35, 0);
    if (!fonte) { al_destroy_display(janela); fprintf(stderr, "Falha ao carregar fonte.\n"); return false; }

    //cria os bitmaps dos quadrados
    seta_quadrados(quadrados, tamanho);

    //colore os quadrados
    colorir(quadrados, tamanho);

    // volta para janela
    al_set_target_bitmap(al_get_backbuffer(janela));

    al_flip_display();
    //loop de palavras
    for (i = 0; i < 3; i++) {
        qtd = 0;
        quadrado1 = false;
        quadrado2 = false;
        quadrado3 = false;
        quadrado4 = false;
        quadrado5 = false;
        //inicializar fila
        fila = al_create_event_queue();
        if (!fila) {
            fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
            al_destroy_display(janela);
            return false;
        }
        al_register_event_source(fila, al_get_mouse_event_source());

        //retira uma palavra aleatoria
        palavra_atual = getPalavra(dicionario);
        strcpy(auxiliar, palavra_atual.palavra);

        //faz o shuffle na palavra
        embaralha(auxiliar);

        //loop de jogabilidade
        while (qtd < tamanho) {
            while (!al_is_event_queue_empty(fila)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila, &evento);
                //se houve um click
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    //primeiro quadrado
                    if (quadrado1 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[0]) &&
                        evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[0])) {
                        palavra[qtd] = auxiliar[1];
                        quadrado1 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 1;
                        som_clicar();
                    }
                        //segundo quadrado a direita
                    else if (quadrado2 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[1]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[1])) {
                        palavra[qtd] = auxiliar[3];
                        quadrado2 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 2;
                        som_clicar();
                    }
                        //terceiro quadrado
                    else if (quadrado3 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[2]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[2]) &&
                             evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[2]) &&
                             evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[2])) {
                        palavra[qtd] = auxiliar[0];
                        quadrado3 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 3;
                        som_clicar();
                    }
                        //quarto quadrado
                    else if (quadrado4 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[3]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[3]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[3]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[3])) {
                        palavra[qtd] = auxiliar[2];
                        quadrado4 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 4;
                    }
                    else if (quadrado5 == false && evento.mouse.x >= larg2 +100 - al_get_bitmap_width(quadrados[4]) &&
                             evento.mouse.x <= larg2 +100 + al_get_bitmap_width(quadrados[4]) &&
                             evento.mouse.y >= alt1 + 80 - al_get_bitmap_height(quadrados[4]) &&
                             evento.mouse.y <= alt1 + 80 + al_get_bitmap_height(quadrados[4])) {
                        palavra[qtd] = auxiliar[4];
                        quadrado5 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 5;
                    }
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return false;
                }
            }

            al_clear_to_color(al_map_rgb(255, 255, 200));
            ////define a cor para os quadrados
            //colorir(um, dois, tres, quatro);
            //volta para janela
            al_set_target_bitmap(al_get_backbuffer(janela));
            //Printa a pontuacao e a quantidade de vida
            exibe_legendas(pontuacao, Usuario.vidas, fonte);

            //printa as letras e caso ja tenha sido selecionadas, andam com o quadrado

            if (quadrado1 == false) {
                al_draw_bitmap(quadrados[0], larg1, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[1]);
            } else {
                posicao = busca_posicao(coordenadas, 1, tamanho);
                al_draw_bitmap(quadrados[0], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[1]);
                al_rest(0.2);
            }

            if (quadrado2 == false) {
                al_draw_bitmap(quadrados[1], larg1, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[3]);
            } else {
                posicao = busca_posicao(coordenadas, 2, tamanho);
                al_draw_bitmap(quadrados[1], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[3]);
                al_rest(0.2);
            }

            if (quadrado3 == false) {
                al_draw_bitmap(quadrados[2], larg2, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[0]);
            } else {
                posicao = busca_posicao(coordenadas, 3, tamanho);
                al_draw_bitmap(quadrados[2], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[0]);
                al_rest(0.2);
            }

            if (quadrado4 == false) {
                al_draw_bitmap(quadrados[3], larg2, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[2]);
            } else {
                posicao = busca_posicao(coordenadas, 4, tamanho);
                al_draw_bitmap(quadrados[3], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[2]);
                al_rest(0.2);
            }

            if (quadrado5 == false) {
                al_draw_bitmap(quadrados[4], larg2+100, alt1+80, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 125, alt1 +90, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[4]);
            } else {
                posicao = busca_posicao(coordenadas, 5, tamanho);
                al_draw_bitmap(quadrados[4], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[4]);
                al_rest(0.2);
            }
            al_flip_display();
        }

        //atualizao display com as letras
        al_flip_display();

        //verifica se o usuario escolheu a ordem correta
        if (strcmp(palavra, palavra_atual.palavra) == 0) {
            palavra_correta(fonte, palavra);
            pontuacao++;
        }
            //palavra errada
        else {
            Errou(palavra_atual, pontuacao);
            Usuario.recorde = pontuacao;
            registrar_usuario(Usuario);
            return false;
        }
    }
    return true;
}


bool modo4() {
    ALLEGRO_BITMAP *quadrados[4];
    ALLEGRO_FONT *fonte;
    Tusuario Usuario; Usuario.vidas = 0;
    Tpalavra dicionario[6], palavra_atual;
    bool continua = false;
    int tamanho = 4;
    int i, qtd = 0, coordenadas[4][2], posicao = 0;
    bool  quadrado1 = false, quadrado2 = false, quadrado3 = false, quadrado4 = false;
    char auxiliar[8], palavra[8];
    float larg1 = 200, alt1 = 130, larg2 = 400, alt2 = 280;

    //para setar os quadrados depois
    preenche_coordenada(coordenadas, tamanho);//zera o vetor com as posicoes posteriores dos quadrados

    //passa todas as palavras do arquivo para o dicionario
    toVetor(dicionario, arq_palavras_tamanho4);
    strcpy(nome, "");
    fonte = al_load_font("queen.ttf", 35, 0);
    if (!fonte) { al_destroy_display(janela); fprintf(stderr, "Falha ao carregar fonte.\n"); return false; }

    //cria os bitmaps dos quadrados
    seta_quadrados(quadrados, tamanho);

    //colore os quadrados
    colorir(quadrados, tamanho);

    // volta para janela
    al_set_target_bitmap(al_get_backbuffer(janela));

    al_flip_display();
    //loop de palavras
    for (i = 0; i < 5; i++) {
        qtd = 0;
        quadrado1 = false;
        quadrado2 = false;
        quadrado3 = false;
        quadrado4 = false;
        //inicializar fila
        fila = al_create_event_queue();
        if (!fila) {
            fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
            al_destroy_display(janela);
            return false;
        }
        al_register_event_source(fila, al_get_mouse_event_source());

        //retira uma palavra aleatoria
        palavra_atual = getPalavra(dicionario);
        strcpy(auxiliar, palavra_atual.palavra);

        //faz o shuffle na palavra
        embaralha(auxiliar);

        //loop de jogabilidade
        while (qtd < tamanho) {
            while (!al_is_event_queue_empty(fila)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila, &evento);
                //se houve um click
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    //primeiro quadrado
                    if (quadrado1 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[0]) &&
                        evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[0])) {
                        palavra[qtd] = auxiliar[1];
                        quadrado1 = 1;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 1;
                        som_clicar();
                    }
                        //segundo quadrado a direita
                    else if (quadrado2 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[1]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[1])) {
                        palavra[qtd] = auxiliar[3];
                        quadrado2 = 1;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 2;
                        som_clicar();
                    }
                        //terceiro quadrado
                    else if (quadrado3 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[2]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[2]) &&
                             evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[2]) &&
                             evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[2])) {
                        palavra[qtd] = auxiliar[0];
                        quadrado3 = 1;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 3;
                        som_clicar();
                    }
                        //quarto quadrado
                    else if (quadrado4 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[3]) &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[3]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[3]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[3])) {
                        palavra[qtd] = auxiliar[2];
                        quadrado4 = 1;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 4;
                    }
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return false;
                }
            }

            al_clear_to_color(al_map_rgb(255, 255, 200));
            ////define a cor para os quadrados
            //colorir(um, dois, tres, quatro);
            //volta para janela
            al_set_target_bitmap(al_get_backbuffer(janela));
            //Printa a pontuacao e a quantidade de vida
            exibe_legendas(pontuacao, Usuario.vidas, fonte);

            //printa as letras e caso ja tenha sido selecionadas, andam com o quadrado

            if (quadrado1 == false) {
                al_draw_bitmap(quadrados[0], larg1, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[1]);
            } else {
                posicao = busca_posicao(coordenadas, 1, tamanho);
                al_draw_bitmap(quadrados[0], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[1]);
                al_rest(0.2);
            }

            if (quadrado2 == false) {
                al_draw_bitmap(quadrados[1], larg1, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[3]);
            } else {
                posicao = busca_posicao(coordenadas, 2, tamanho);
                al_draw_bitmap(quadrados[1], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[3]);
                al_rest(0.2);
            }

            if (quadrado3 == false) {
                al_draw_bitmap(quadrados[2], larg2, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[0]);
            } else {
                posicao = busca_posicao(coordenadas, 3, tamanho);
                al_draw_bitmap(quadrados[2], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[0]);
                al_rest(0.2);
            }

            if (quadrado4 == false) {
                al_draw_bitmap(quadrados[3], larg2, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[2]);
            } else {
                posicao = busca_posicao(coordenadas, 4, tamanho);
                al_draw_bitmap(quadrados[3], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[2]);
                al_rest(0.2);
            }
            al_flip_display();
        }

        //atualizao display com as letras
        al_flip_display();

        //verifica se o usuario escolheu a ordem correta
        if (strcmp(palavra, palavra_atual.palavra) == 0) {
            palavra_correta(fonte, palavra);
            pontuacao++;
        }
            //palavra errada
        else {
            Errou(palavra_atual, pontuacao);
            Usuario.recorde = pontuacao;
            registrar_usuario(Usuario);
            return false;
        }
    }
}


bool modo3() {
    ALLEGRO_BITMAP *quadrados[3];
    ALLEGRO_FONT *fonte;
    Tusuario Usuario; Usuario.vidas = 0;
    Tpalavra dicionario[6], palavra_atual;
    int tamanho = 3;
    int i, qtd = 0, coordenadas[3][2], posicao = 0;
    bool  quadrado1 = false, quadrado2 = false, quadrado3 = false;
    char auxiliar[5], palavra[5];
    float larg1 = 200, alt1 = 130, larg2 = 400, alt2 = 280;

    //para setar os quadrados depois
    preenche_coordenada(coordenadas, tamanho);//zera o vetor com as posicoes posteriores dos quadrados

    //passa todas as palavras do arquivo para o dicionario
    toVetor(dicionario, arq_palavras_tamanho3);
    strcpy(nome, "");
    fonte = al_load_font("queen.ttf", 35, 0);
    if (!fonte) { al_destroy_display(janela); fprintf(stderr, "Falha ao carregar fonte.\n"); return false; }

    //cria os bitmaps dos quadrados
    seta_quadrados(quadrados, tamanho);

    //colore os quadrados
    colorir(quadrados, tamanho);

    // volta para janela
    al_set_target_bitmap(al_get_backbuffer(janela));

    al_flip_display();
    //loop de palavras
    for (i = 0; i < 5; i++) {
        qtd = 0;
        quadrado1 = false;quadrado2 = false;quadrado3 = false;
        //inicializar fila
        fila = al_create_event_queue();
        if (!fila) { fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");al_destroy_display(janela); return false; }
        al_register_event_source(fila, al_get_mouse_event_source());

        //retira uma palavra aleatoria
        palavra_atual = getPalavra(dicionario);
        strcpy(auxiliar, palavra_atual.palavra);

        //faz o shuffle na palavra
        embaralha(auxiliar);

        //loop de jogabilidade
        while (qtd < tamanho) {
            while (!al_is_event_queue_empty(fila)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila, &evento);
                //se houve um click
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    //primeiro quadrado
                    if (quadrado1 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[0]) &&
                        evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[0]) &&
                        evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[0])) {
                        palavra[qtd] = auxiliar[1];
                        quadrado1 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 1;
                        som_clicar();
                    }
                        //segundo quadrado a direita
                    else if (quadrado2 == false && evento.mouse.x >= larg1 - al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.x <= larg1 + al_get_bitmap_width(quadrados[1]) &&
                             evento.mouse.y >= alt2 - al_get_bitmap_height(quadrados[1]) &&
                             evento.mouse.y <= alt2 + al_get_bitmap_height(quadrados[1])) {
                        palavra[qtd] = auxiliar[2];
                        quadrado2 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 2;
                        som_clicar();
                    }
                        //terceiro quadrado
                    else if (quadrado3 == false && evento.mouse.x >= larg2 - al_get_bitmap_width(quadrados[2]) - 90 &&
                             evento.mouse.x <= larg2 + al_get_bitmap_width(quadrados[2]) + 90 &&
                             evento.mouse.y >= alt1 - al_get_bitmap_height(quadrados[2]) &&
                             evento.mouse.y <= alt1 + al_get_bitmap_height(quadrados[2])) {
                        palavra[qtd] = auxiliar[0];
                        quadrado3 = true;
                        qtd++;
                        palavra[qtd] = '\0';
                        coordenadas[qtd - 1][1] = 3;
                        som_clicar();
                    }
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return false;
                }
            }

            al_clear_to_color(al_map_rgb(255, 255, 200));
            ////define a cor para os quadrados
           // colorir(quadrados,tamanho);

            //volta para janela
            al_set_target_bitmap(al_get_backbuffer(janela));
            //Printa a pontuacao e a quantidade de vida
            exibe_legendas(pontuacao, Usuario.vidas, fonte);

            //printa as letras e caso ja tenha sido selecionadas, andam com o quadrado

            if (quadrado1 == false) {
                al_draw_bitmap(quadrados[0], larg1, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[1]);
            } else {
                posicao = busca_posicao(coordenadas, 1, tamanho);
                al_draw_bitmap(quadrados[0], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[1]);
                al_rest(0.2);
            }

            if (quadrado2 == false) {
                al_draw_bitmap(quadrados[1], larg1, alt2, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg1 + 26, alt2 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[2]);
            } else {
                posicao = busca_posicao(coordenadas, 2, tamanho);
                al_draw_bitmap(quadrados[1], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[2]);
                al_rest(0.2);
            }

            if (quadrado3 == false) {
                al_draw_bitmap(quadrados[2], larg2 - 90, alt1, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), larg2 + 26 - 90, alt1 + 12, ALLEGRO_ALIGN_CENTER, "%c",
                              auxiliar[0]);
            } else {
                posicao = busca_posicao(coordenadas, 3, tamanho);
                al_draw_bitmap(quadrados[2], coordenadas[posicao][0] + 170, 34, 0);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), coordenadas[posicao][0] + 27 + 170, 34 + 10,
                              ALLEGRO_ALIGN_CENTER, "%c", auxiliar[0]);
                al_rest(0.2);
            }
            al_flip_display();
        }

        //atualizao display com as letras
        al_flip_display();

        //verifica se o usuario escolheu a ordem correta
        if (strcmp(palavra, palavra_atual.palavra) == 0) {
            palavra_correta(fonte, palavra);
            pontuacao++;
        }
            //palavra errada
        else {
            Errou(palavra_atual, pontuacao);
            Usuario.recorde = pontuacao;
            registrar_usuario(Usuario);
            return false;
        }
    }
    return true;
}


void seta_quadrados(ALLEGRO_BITMAP* quadrados[], int tamanho) {
    for (int i = 0; i < tamanho; i++)
    {
        quadrados[i] = NULL;
        quadrados[i] = al_create_bitmap(50, 50);
    }
}

void palavra_correta(ALLEGRO_FONT *fonte, char palavra[]) {
    al_clear_to_color(al_map_rgb(255, 255, 200));
    al_set_audio_stream_playing(som_fundo, false);
    al_rest(0.3);
    al_play_sample(som_acertou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "PALAVRA CORRETA!  %s", palavra);
    al_flip_display();
    al_rest(0.8);
    al_set_audio_stream_playing(som_fundo, true);
}

bool checkPoint() {
    TAudio audio;
    ALLEGRO_FONT *fonte = NULL;
    bool sair = false, concluido = false;
    char palavra[20];
    inicializa_palavras_do_checkPoint();
    strcpy(palavra, "");
    audio = getAudio();
    fonte = al_load_font("queen.ttf", 37, 0);
    if (!fonte) { al_destroy_display(janela);	fprintf(stderr, "Falha ao carregar fonte.\n");	return -1; }
    al_clear_to_color(al_map_rgb(255, 255, 200));
    al_draw_text(fonte, al_map_rgb(0, 0, 0), 30, 50, 0, "Parabens! Voce chegou ao Check Point!");
    al_draw_text(fonte, al_map_rgb(0, 0, 0), 30, 110, 0, "Aperte o play para reproduzir um audio ");
    al_draw_text(fonte, al_map_rgb(0, 0, 0), 30, 150, 0, "que voce devera escreve-lo corretamente!");
    al_flip_display();
    al_rest(1.0);
    al_clear_to_color(al_map_rgb(255, 255, 200));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_set_audio_stream_playing(som_fundo, false);
    while (!sair) {
        while (!al_is_event_queue_empty(fila)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);
            if (!concluido) {
                //al_clear_to_color(al_map_rgb(255, 255, 200));
                al_draw_bitmap(botao_play, LARGURA_TELA / 2, ALTURA_TELA / 2, 0);
                al_draw_text(fonte, al_map_rgb(0, 0, 0), 30, 100, 0, "Digite:");
                manipular_entrada(evento, palavra);
                al_draw_text(fonte, al_map_rgb(0, 0, 0), 150, 100, 0, palavra);
                al_flip_display();

                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    concluido = true;
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    sair = true;
                }
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_play) && evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_play) && evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_play) && evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_play)) {
                        al_rest(1.0);
                        al_play_sample(audio.audio, 1.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_rest(1.0);
                    }
                }
            }
        }
        al_rest(1.0);
        if (concluido) { break; }
        al_set_audio_stream_playing(som_fundo, true);
    }
    if (strcmp(palavra, audio.palavra) == 0) {
        al_clear_to_color(al_map_rgb(255, 255, 200));
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 - 280, ALTURA_TELA / 2 - 200, 0, "Muito bem! Voce pode prosseguir no jogo!");
        al_flip_display();
        al_rest(1.0);
        return true;
    }
    errou_checkpoint(audio.palavra);
    return false;
}

void Errou(Tpalavra palavra_atual, int pontuacao) {
    bool sair = false, concluido = false;
    al_register_event_source(fila, al_get_keyboard_event_source());
    ALLEGRO_FONT *fonte;
    fonte = al_load_font("queen.ttf", 40, 0);
    if (!fonte) {
        al_destroy_display(janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return;
    }
    printf("Palavra incorreta!");
    al_set_audio_stream_playing(som_fundo, false);
    explicacao(palavra_atual, fonte);
    al_rest(2.0);
    al_set_audio_stream_playing(som_fundo, true);
    while (!sair) {
        while (!al_is_event_queue_empty(fila)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);
            if (!concluido) {
                al_clear_to_color(al_map_rgb(255, 255, 200));
                al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 70, 0,
                             "Digite seu nome: ");
                al_flip_display();
                manipular_entrada(evento, nome);

                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    concluido = true;
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    sair = true;
                }
            }
        }
        exibir_texto_centralizado(fonte);
        al_flip_display();
        if (concluido)
            break;
    }
    if (strlen(nome) > 0) {
        al_clear_to_color(al_map_rgb(255, 255, 200));
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER,
                      "%s - %d PONTOS", nome, pontuacao);
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 + 80, ALLEGRO_ALIGN_CENTER,
                     "Obrigado por jogar!");
        al_flip_display();
        al_rest(1.0);

    } else {
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER,
                     "Letras nao digitadas!");
    }
}

void errou_checkpoint(char palavra[]){
    bool sair=false, concluido=false;
    ALLEGRO_FONT *fonte = NULL;
    fonte = al_load_font("queen.ttf", 27, 0);
    Tusuario usuario;
    usuario.recorde=pontuacao;
    strcpy(nome,"");

    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 70, ALLEGRO_ALIGN_CENTER,
                 "A palavra correta era: %s",palavra);
    while (!sair) {
        while (!al_is_event_queue_empty(fila)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);
            if (!concluido) {
                al_clear_to_color(al_map_rgb(255, 255, 200));
                al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 70, 0,
                             "Digite seu nome: ");
                al_flip_display();
                manipular_entrada(evento, nome);

                if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    concluido = true;
                }
                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    sair = true;
                }
            }
        }
        exibir_texto_centralizado(fonte);
        al_flip_display();
        if (concluido)
            break;
    }
    if (strlen(nome) > 0) {
        al_clear_to_color(al_map_rgb(255, 255, 200));
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER,
                      "%s - %d PONTOS", nome, usuario.recorde);
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 + 80, ALLEGRO_ALIGN_CENTER,
                     "Obrigado por jogar!");
        al_flip_display();
        registrar_usuario(usuario);

    } else {
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER,
                     "Letras nao digitadas!");
    }
}

void finalizar() {
    al_destroy_sample(som_acertou);
    al_destroy_sample(som_errou);
    al_destroy_audio_stream(som_fundo);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);
    fclose(arq_usuarios);
}

FILE * criarArquivo(char nome[]) {
    FILE * arq;
    arq = fopen(nome, "r+b");
    if (arq == NULL) {
        arq = fopen(nome, "w+b");
    }
    return arq;
}

void menu_jogo(ALLEGRO_FONT *fonte) {
    bool sair=false, concluido=false;
    fonte = al_load_font("queen.ttf", 30, 0);
    if (!fonte) { al_destroy_display(janela); fprintf(stderr, "Falha ao carregar fonte.\n"); return; }
    fila = al_create_event_queue();
    if (!fila) { fprintf(stderr, "Falha ao criar fila de eventos.\n"); al_destroy_display(janela); return; }
    instrucao();
    al_clear_to_color(al_map_rgb(255, 255, 200));
    pontuacao=0;

    if(modo3()) {
        if (checkPoint()) {
            if (modo4()) {
                if (checkPoint()) {
                    if (modo5()) {
                        if (checkPoint()) {
                            if (modo6()) {
                                al_register_event_source(fila,al_get_keyboard_event_source());
                                al_draw_text(fonte, al_map_rgb(0, 0, 0), 100, 200, 0, "Parabens! Voce venceu!");
                                al_rest(2.0);
                                Tusuario usuario;
                                usuario.recorde=15;
                                strcpy(nome,"");
                                while (!sair) {
                                    while (!al_is_event_queue_empty(fila)) {
                                        ALLEGRO_EVENT evento;
                                        al_wait_for_event(fila, &evento);
                                        if (!concluido) {
                                            al_clear_to_color(al_map_rgb(255, 255, 200));
                                            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 70, 0,
                                                         "Digite seu nome: ");
                                            al_flip_display();
                                            manipular_entrada(evento, nome);

                                            if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                                                concluido = true;
                                            }
                                            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                                                sair = true;
                                            }
                                        }
                                    }
                                    exibir_texto_centralizado(fonte);
                                    al_flip_display();
                                    if (concluido)
                                        break;
                                }
                                if (strlen(nome) > 0) {
                                    al_clear_to_color(al_map_rgb(255, 255, 200));
                                    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER,
                                                  "%s - %d PONTOS", nome, usuario.recorde);
                                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 + 80, ALLEGRO_ALIGN_CENTER,
                                                 "Obrigado por jogar!");
                                    al_flip_display();

                                } else {
                                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER,
                                                 "Letras nao digitadas!");
                                }
                                registrar_usuario(usuario);

                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    al_draw_text(fonte, al_map_rgb(0, 0, 0), 100, 200, 0, "Tente outra vez!");

}


//Area de palavras
void embaralha(char word[]) {
    int i, index;
    srand((unsigned)time(NULL));
    for (i = 0; i < strlen(word); i++) {
        index = rand() % strlen(word);
        char temp = word[i];
        word[i] = word[index];
        word[index] = temp;
    }
    word[strlen(word)] = '\0';
}

void toVetor(Tpalavra dicionario[], FILE*arq_palavras) {
    int status, i = 0;
    Tpalavra p;
    fseek(arq_palavras, 0, 0);
    while (i < PALAVRAS_SALVAS) {
        status = fread(&p, sizeof(Tpalavra), 1, arq_palavras);
        if (status != 1) { fprintf(stderr, "Falha na leitura\n"); return; }
        dicionario[i] = p;
        i++;
    }
}

Tpalavra getPalavra(Tpalavra dicionario[]) {
    int index = 0;
    bool valido = false;
    srand((unsigned)time(NULL));
    while (!valido) {
        index = rand() % PALAVRAS_SALVAS;
        if (dicionario[index].status == 1) {
            valido = true;
            dicionario[index].status = 0;
        }
    }
    return dicionario[index];
}

void cadastro_de_palavras(FILE *arquivo_palavras) {
    Tpalavra palavra;
    int status = 0;
    fseek(arquivo_palavras, 0, 2);
    for (int i = 0; i < PALAVRAS_SALVAS; i++) {
        printf("\nDigite a palavra: ");
        gets(palavra.palavra); setbuf(stdin, NULL);
        printf("\nDigite a traducao: ");
        gets(palavra.traducao); setbuf(stdin, NULL);
        printf("\nDigite a descricao: ");
        gets(palavra.descricao); setbuf(stdin, NULL);
        palavra.status = 1;

        status = fwrite(&palavra, sizeof(Tpalavra), 1, arquivo_palavras);
        if (status != 1) { printf("Erro gravacao\n"); return; }
    }
}

void explicacao(Tpalavra palavra_atual, ALLEGRO_FONT *fonte) {
    al_clear_to_color(al_map_rgb(255, 255, 200));
    al_play_sample(som_errou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 80, ALLEGRO_ALIGN_CENTER, "A palavra correta eh: %s", palavra_atual.palavra);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, ALLEGRO_ALIGN_CENTER, "Traducao: %s", palavra_atual.traducao);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "Significado:  %s", palavra_atual.descricao);
    al_flip_display();
    printa_seta(seta_direita, 300, 300);
}

void inicializa_palavras_do_checkPoint() {
    strcpy(lista_audio[0].palavra, "street");
    strcpy(lista_audio[1].palavra, "yellow");
    strcpy(lista_audio[2].palavra, "water");
    strcpy(lista_audio[3].palavra, "plastic");
    strcpy(lista_audio[4].palavra, "pencil");
    strcpy(lista_audio[5].palavra, "pants");
    strcpy(lista_audio[6].palavra, "package");
    strcpy(lista_audio[7].palavra, "house");
    strcpy(lista_audio[8].palavra, "door");
    strcpy(lista_audio[9].palavra, "book");
}





//background parallax
void InitBG(Tback *back, float x, float y, float velX, float velY, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *imagem) {
    back->x = x;
    back->y = y;
    back->altura = altura;
    back->largura = largura;
    back->velocidade_x = velX;
    back->velocidade_y = velY;
    back->dirX = dirX;
    back->dirY = dirY;
    back->backGround = imagem;
}

void atualizarBG(Tback *back) {
    back->x += back->velocidade_x * back->dirX;
    if (back->x + back->largura <= 0) {
        back->x = 0;
    }
}

void printaBG(Tback *back) {
    al_draw_bitmap(back->backGround, back->x, back->y, 0);
    if (back->x + back->largura < LARGURA_TELA) {
        al_draw_bitmap(back->backGround, back->x + back->largura, back->y, 0);
    }
}


//Configuracoes
bool inicializa() {
    // Inicializacao da biblioteca Allegro
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }

    // Inicializacao do add-on para uso de fontes
    al_init_font_addon();

    // Inicializacao do add-on para uso de fontes True Type
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    //Inicializacao do add-on de imagens
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar o allegro imagem\n");
        return false;
    }

    // Criacao da janela
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    // Torna apto o uso de mouse na aplica��o
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return false;
    }

    // Atribui o cursor padr�o do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return false;
    }
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }
    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar �udio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de �udio.\n");
        return false;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de �udio.\n");
        return false;
    }

    fila = al_create_event_queue();
    if (!fila) {
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }

    al_register_event_source(fila, al_get_display_event_source(janela));

    return true;
}

bool inicializa_imagens() {
    imagem_fundo = al_load_bitmap("background.png");
    if (!imagem_fundo) {
        fprintf(stderr, "falha");
        return false;
    }
    imagem_coracao = al_load_bitmap("coracao.png");
    if (!imagem_coracao) {
        fprintf(stderr, "falha");
        return false;
    }
    imagem_podio = al_load_bitmap("podio.png");
    if (!imagem_podio) {
        fprintf(stderr, "falha");
        return false;
    }
    seta_direita = al_load_bitmap("setaDireita.png");
    if (!seta_direita) {
        fprintf(stderr, "Falha na imagem");
        return false;
    }
    seta_esquerda = al_load_bitmap("setaEsquerda.png");
    if (!seta_esquerda) {
        fprintf(stderr, "Falha na imagem");
        return false;
    }
    botao_play = al_load_bitmap("play.png");
    if (!botao_play) {
        fprintf(stderr, "Falha na imagem");
        return false;
    }
    return true;

}

bool inicializa_audios() {
    som_acertou = al_load_sample("acertou.ogg");
    if (!som_acertou)
    {
        fprintf(stderr, "Falha ao carregar sample.\n");
        al_destroy_display(janela);
        return false;
    }
    som_errou = al_load_sample("faustao.ogg");
    if (!som_errou)
    {
        fprintf(stderr, "Falha ao carregar sample.\n");
        al_destroy_display(janela);
        return false;
    }
    som_click = al_load_sample("clicar.wav");
    if (!som_click) {
        fprintf(stderr, "Falha no sample");
        al_destroy_display(janela);
        return false;
    }
    som_fundo = al_load_audio_stream("fundo.ogg", 4, 1024);
    if (!som_fundo) {
        fprintf(stderr, "Falha ao carregar o sample\n");
        al_destroy_display(janela);
        return false;
    }

    lista_audio[0].audio = al_load_sample("street.ogg");
    lista_audio[1].audio = al_load_sample("yellow.ogg");
    lista_audio[2].audio = al_load_sample("water.ogg");
    lista_audio[3].audio = al_load_sample("plastic.ogg");
    lista_audio[4].audio = al_load_sample("pencil.ogg");
    lista_audio[5].audio = al_load_sample("pants.ogg");
    lista_audio[6].audio = al_load_sample("package.ogg");
    lista_audio[7].audio = al_load_sample("house.ogg");
    lista_audio[8].audio = al_load_sample("door.ogg");
    lista_audio[9].audio = al_load_sample("book.ogg");

    for (int i = 0; i < 10; i++) {
        if (!lista_audio[i].audio) {
            fprintf(stderr, "Falha no sample");
            al_destroy_display(janela);
            return false;
        }

    }
    return true;
}


//interaçao com o usuario
void manipular_entrada(ALLEGRO_EVENT evento, char palavra[]) {
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
        if (strlen(palavra) <= 10) {
            char temp[] = { evento.keyboard.unichar, '\0' };
            if (evento.keyboard.unichar == ' ')
            {
                strcat(palavra, temp);
            }
            else if (evento.keyboard.unichar >= '0' &&
                     evento.keyboard.unichar <= '9')
            {
                strcat(palavra, temp);
            }
            else if (evento.keyboard.unichar >= 'A' &&	evento.keyboard.unichar <= 'Z') {
                strcat(palavra, temp);
            }
            else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z') {
                strcat(palavra, temp);
            }
        }

        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(palavra) != 0) {
            palavra[strlen(palavra) - 1] = '\0';
        }
    }
}

void exibir_texto_centralizado(ALLEGRO_FONT *fonte) {
	/*ALLEGRO_FONT *fonte;
	fonte = al_load_font("queen.ttf", 40, 0);*/
	if (!fonte) {
		al_destroy_display(janela);
		fprintf(stderr, "Falha ao carregar fonte.\n");
		return;
	}
    if (strlen(nome) > 0)
        al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 + 80, ALTURA_TELA / 2 - 70, ALLEGRO_ALIGN_CENTRE, nome);
}

void registrar_usuario(Tusuario Usuario) {
    int status = 0;
    strcpy(Usuario.nome_usuario, nome);
    fseek(arq_usuarios, 0, SEEK_END);
    status = fwrite(&Usuario, sizeof(Tusuario), 1, arq_usuarios);
    if (status != 1) {
        printf("Erro de gravacao\n");
        return;
    }
}


//Funcoes dos quadrados
void colorir(ALLEGRO_BITMAP*quadrados[], int tamanho) {
    for (int i = 0; i < tamanho; i++)
    {
        al_set_target_bitmap(quadrados[i]);
        al_clear_to_color(al_map_rgb(210, 105, 30));
    }
}

void preenche_coordenada(int matriz[][2], int tamanho) {
    int i, a = 52;
    for (i = 0; i < tamanho; i++) {
        matriz[i][0] = a;
        matriz[i][1] = 0;
        a += 45;
    }
}

int busca_posicao(int matriz[][2], int quadrado, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        if (matriz[i][1] == quadrado) {
            return i;
        }
    }
    return 0;
}


//Prints
void exibe_legendas(int pontuacao, int vidas, ALLEGRO_FONT *fonte) {
    //exibe o coracao
  //  al_draw_bitmap(imagem_coracao, LARGURA_TELA / 2 + 80, ALTURA_TELA / 2 + 190, 0);
    //exbie o podio
    al_draw_bitmap(imagem_podio, LARGURA_TELA / 2 + 80, ALTURA_TELA / 2 + 170, 0);
    //exibe a pontuacao
    al_draw_textf(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 + 192, ALTURA_TELA / 2 + 180, ALLEGRO_ALIGN_CENTER, "Pontuacao: %d", pontuacao);

    //exibe quantidade de vidas extras
   // al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2 + 200, ALTURA_TELA / 2 + 190, ALLEGRO_ALIGN_CENTER, "Vidas extras: %d", vidas);

}

void printa_menu(ALLEGRO_FONT *fonte, bool sair, bool recordes, bool iniciar) {
    al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 200, ALLEGRO_ALIGN_CENTER, "Joguinho das Palavra");
    al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2, ALTURA_TELA / 3 + 10, ALLEGRO_ALIGN_CENTER, "Jogar");
    al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2, ALTURA_TELA / 3 + 220, ALLEGRO_ALIGN_CENTER, "Sair");
    al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2, ALTURA_TELA / 3 + 125, ALLEGRO_ALIGN_CENTER, "Recordes");
    al_flip_display();

    if (iniciar == true) {
        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 10, ALLEGRO_ALIGN_CENTER, "Jogar");
        al_flip_display();
        al_rest(1.0);
        menu_jogo(fonte);
    }
    else if (sair == true) {
        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 220, ALLEGRO_ALIGN_CENTER, "Sair");
        al_flip_display();
        al_rest(1.0);
    }
    else if (recordes == true) {
        al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 3 + 125, ALLEGRO_ALIGN_CENTER, "Recordes");
        al_flip_display();
        al_rest(1.0);
        exibir_recordes();
    }
}

void printa_seta(ALLEGRO_BITMAP *seta, int x, int y) {
    while (1) {
        al_draw_bitmap(seta, x, y, 0);
        al_flip_display();
        while (!al_is_event_queue_empty(fila)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (evento.mouse.x >= x - al_get_bitmap_width(seta) && evento.mouse.x <= x + al_get_bitmap_width(seta) && evento.mouse.y >= y - al_get_bitmap_height(seta) && evento.mouse.y <= y + al_get_bitmap_height(seta)) {
                    return;
                }
            }
        }
    }
}


//Sons
void som_clicar() {
    //al_set_audio_stream_playing(som_fundo, false);
    al_play_sample(som_click, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    al_rest(0.01);
    //al_set_audio_stream_playing(som_fundo, true);
}

TAudio getAudio() {
    int index;
    srand((unsigned)time(NULL));
    index = rand() % 10;
    return lista_audio[index];
}
