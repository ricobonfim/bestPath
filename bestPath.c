#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "biblga.h"
#include "bestPath.h"


/*
Objetivo:
Rodar as mascaras que o genetico vai gerar
Funcao de fitness diz se a mascara que foi dada foi boa ou ruim!!!!!
*/

/*
cima=0
esquerda=1
baixo=2
direita=3
*/

/*
E de total responsabilidade do usuario inserir um mapa nas condicoes 
aceitaveis do programa definidas abaixo:
mapa deve ter apenas 1 'x' e um 'y'
mapa deve ter ao menos 1 caminho entre os pontos 'x' e 'y'
os unicos caracteres aceitaveis sao: 'x', 'y', ' ', '*'.
o mapa deve estar inteiramente fechado - com * em todos os limites
*/

extern uint8_t **mascara;
extern char nomeArquivo[100];

/*extern struct gaopt {int maxpop;
                     int nger;
                     float pmut;
                     int nbits;
                     unsigned char *best;
                     int nbytes;
                     float *fit;
                     unsigned int *ind;
                     unsigned char *pop;
                     unsigned nshow;     };
*/
struct gaopt g = {100, //Tamanho da popoulacao
                  1000, //Numero de geracoes
                  60,  //Percentual de mutacao
                  80,  //Numero de bits a otimizar
                  0,   //Ponteiro para a melhor solucao
                  0,   //Numero debyte (calcula automaticamente)
                  0,   //Nao usar
                  0,   //Nao usar
                  0,   //Ponteiro para a populacao
                  0    //Numero de solucoes mostradas a cada geracao (acumula na tela)
                   };

/* recebe mapa carregado e imprime na tela
extern void imprimeMapa();
// recebe mapa e coloca as coordenadas do ponto x na estrutura pilha
extern void encontraInicio(pilha_t *pilha);
// percorre o mapa usando a mascara para se orientar nas direcoes e retorna o numero de passos
extern float percorreMapa(uint8_t *p);
// anda uma casa para cima no mapa e atualiza pilha
extern int andaCima(pilha_t *pilha);
// anda uma casa para baixo no mapa e atualiza pilha
extern int andaBaixo(pilha_t *pilha);
// anda uma casa para direita no mapa e atualiza pilha
extern int andaDireita(pilha_t *pilha);
// anda uma casa para esquerda no mapa e atualiza pilha
extern int andaEsquerda(pilha_t *pilha);
// recebe os bytes do genetico e gera 5 pares de mascara
extern void criaMascara(uint8_t *p);*/


int main(int argc, char **argv){
    int i;

    mascara = (uint8_t **) malloc(10*sizeof(uint8_t *));
    mascara[0] = (uint8_t *) malloc(4*sizeof(uint8_t)); 
    mascara[1] = (uint8_t *) malloc(sizeof(uint8_t)); 
    mascara[2] = (uint8_t *) malloc(4*sizeof(uint8_t)); 
    mascara[3] = (uint8_t *) malloc(sizeof(uint8_t)); 
    mascara[4] = (uint8_t *) malloc(4*sizeof(uint8_t)); 
    mascara[5] = (uint8_t *) malloc(sizeof(uint8_t)); 
    mascara[6] = (uint8_t *) malloc(4*sizeof(uint8_t)); 
    mascara[7] = (uint8_t *) malloc(sizeof(uint8_t)); 
    mascara[8] = (uint8_t *) malloc(4*sizeof(uint8_t)); 
    mascara[9] = (uint8_t *) malloc(sizeof(uint8_t)); 


    infoMapa = (info_t *) malloc(sizeof(struct info_s));

    if(argc != 2){
        printf("Faltou o caminho para o arquivo do mapa!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(nomeArquivo, argv[1]);

    g = ga(g, fitness);

    printf("Usando %d bytes.\n\n",g.nbytes);
    for (i=0; i<g.nbytes; i++)
        printf("%d ",g.best[i]);
    fitness(g.best);
    
    printf("Mapa antes do fim:\n");
    imprimeMapa();

    for(i=0;i<=9;i++)
        free(mascara[i]);
    free(mascara);
    free(infoMapa);

    return EXIT_SUCCESS;
}
