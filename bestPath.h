#define MAX_COL 100
#define MAX_LIN 100

typedef struct info_s{
    char caminho[MAX_LIN][MAX_COL]; // ponteiro para as linhas do mapa
    int linhas; // guarda o numero de linhas
    int colunas; // guarda o numero de colunas
}info_t;

info_t *infoMapa;

typedef struct pilha_s{
    int atual;
    int quantidade;
    int *linha;
    int *coluna;
}pilha_t;

uint8_t **mascara;
char nomeArquivo[100];

/* 
funcao percorre o mapa ate encontrar o ponto final ('y')
*/
void percorreMapa(pilha_t *pilha){
    int i, j, contFalha, ret;

    for (i=0, j=0, contFalha=0;;){
        if(j == 4)
            j = 0; // volta para a primeira direcao da mascara
        if(i==10){
            mascara[1][0] = mascara[3][0] = mascara[5][0] = mascara[7][0] = mascara[9][0] = 255;
            i=0;
            continue;
        }
        if(mascara[i+1][0]==0){
            i+=2;
            continue;
        }
        if(mascara[i][j] == 0){
            ret = andaCima(pilha);
            if(ret == -1 || ret == 0){
                contFalha++;
                j++;
            }else if(ret == 1){
                mascara[i+1][0]--;
                contFalha = 0;
            }else if(ret == 2)
                break;
        }else if(mascara[i][j] == 1){
            ret = andaEsquerda(pilha);
            if(ret == -1 || ret == 0){
                contFalha++;
                j++;
            }else if(ret == 1){
                mascara[i+1][0]--;
                contFalha = 0;
            }else if(ret == 2)
                break;
        }else if(mascara[i][j] == 2){
            ret = andaBaixo(pilha);
            if(ret == -1 || ret == 0){
                contFalha++;
                j++;
            }else if(ret == 1){
                mascara[i+1][0]--;
                contFalha = 0;
            }else if(ret == 2)
                break;
        }else if(mascara[i][j] == 3){
            ret = andaDireita(pilha);
            if(ret == -1 || ret == 0){
                contFalha++;
                j++;
            }else if(ret == 1){
                mascara[i+1][0]--;
                contFalha = 0;
            }else if(ret == 2)
                break;
        }else{
            printf("problema com a mascara\n");
            exit(EXIT_FAILURE);
        }
        if(contFalha == 4){
            pilha->atual--;
            contFalha=0;
        }
    } 
}

/* 
funcao imprime mapa carregado na memoria (infoMapa)
*/
void imprimeMapa(){
    int i;
    getc(stdout);
    system("clear");
    for(i=0; i<infoMapa->linhas; i++) 
        printf("\t\t\t\t\t\t%s", infoMapa->caminho[i]);      
}


/* 
funcao percorre coluna por coluna, linha por linha ate encontrar um x no mapa, 
o qual ele considera a posicao inicial do robo
*/
void encontraInicio(pilha_t *pilha){
    int i, j;

    for (i=0; i<infoMapa->linhas; i++){
        for(j=0; j<infoMapa->colunas; j++){
            if(infoMapa->caminho[i][j] == 'x'){
                pilha->linha[0] = i;
                pilha->coluna[0] = j;
                return;
            }
        }
    }
}

/* 
Funcao movimenta 'robo' para direcao definida em seu nome
retorno revela se encontrou, nao andou ou andou
caso funcao encontre um caractere diferente dos esperados (y,*,x,' ') deve interromper a execucao
*/
int andaCima(pilha_t *pilha){
    int atual = pilha->atual;
    if(infoMapa->caminho[pilha->linha[atual]-1][pilha->coluna[atual]] == 'y')
        return 2; //achou
    else if (infoMapa->caminho[pilha->linha[atual]-1][pilha->coluna[atual]] == '*')
        return 0; //limite, nao pode andar mais nesta direcao
    else if (infoMapa->caminho[pilha->linha[atual]-1][pilha->coluna[atual]] == 'x')
        return -1; //posicao ja caminhada
    else if (infoMapa->caminho[pilha->linha[atual]-1][pilha->coluna[atual]] == ' '){
        infoMapa->caminho[pilha->linha[atual]-1][pilha->coluna[atual]] = 'x';
        pilha->linha[atual+1] = pilha->linha[atual]-1;
        pilha->coluna[atual+1] = pilha->coluna[atual];
        pilha->atual++;
        pilha->quantidade++;
        return 1; //sucesso, andou
    }else{
        imprimeMapa();
        printf("simbolo desconhecido no mapa\nsimbolo: '%c'", infoMapa->caminho[pilha->linha[atual]-1][pilha->coluna[atual]]);
        exit(EXIT_FAILURE);
    }
}

/* 
Funcao movimenta 'robo' para direcao definida em seu nome
retorno revela se encontrou, nao andou ou andou
caso funcao encontre um caractere diferente dos esperados (y,*,x,' ') deve interromper a execucao
*/
int andaBaixo(pilha_t *pilha){
    int atual = pilha->atual;
    if(infoMapa->caminho[pilha->linha[atual]+1][pilha->coluna[atual]] == 'y')
        return 2; //achou
    else if (infoMapa->caminho[pilha->linha[atual]+1][pilha->coluna[atual]] == '*')
        return 0; //limite, nao pode andar mais nesta direcao
    else if (infoMapa->caminho[pilha->linha[atual]+1][pilha->coluna[atual]] == 'x')
        return -1; //posicao ja caminhada
    else if (infoMapa->caminho[pilha->linha[atual]+1][pilha->coluna[atual]] == ' '){
        infoMapa->caminho[pilha->linha[atual]+1][pilha->coluna[atual]] = 'x';
        pilha->linha[atual+1] = pilha->linha[atual]+1;
        pilha->coluna[atual+1] = pilha->coluna[atual];
        pilha->atual++;
        pilha->quantidade++;
        return 1; //sucesso, andou
    }else{
        imprimeMapa();
        printf("simbolo desconhecido no mapa\nsimbolo: '%c'", infoMapa->caminho[pilha->linha[atual]+1][pilha->coluna[atual]]);
        exit(EXIT_FAILURE);
    }
}

/* 
Funcao movimenta 'robo' para direcao definida em seu nome
retorno revela se encontrou, nao andou ou andou
caso funcao encontre um caractere diferente dos esperados (y,*,x,' ') deve interromper a execucao
*/
int andaDireita(pilha_t *pilha){
    int atual = pilha->atual;
    if(infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]+1] == 'y')
        return 2; //achou
    else if (infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]+1] == '*')
        return 0; //limite, nao pode andar mais nesta direcao
    else if (infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]+1] == 'x')
        return -1; //posicao ja caminhada
    else if (infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]+1] == ' '){
        infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]+1] = 'x';
        pilha->linha[atual+1] = pilha->linha[atual];
        pilha->coluna[atual+1] = pilha->coluna[atual]+1;
        pilha->atual++;
        pilha->quantidade++;
        return 1; //sucesso, andou
    }else{
        imprimeMapa();
        printf("simbolo desconhecido no mapa\nsimbolo: '%c'", infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]+1]);
        exit(EXIT_FAILURE);
    }
}

/* 
Funcao movimenta 'robo' para direcao definida em seu nome
retorno revela se encontrou, nao andou ou andou
caso funcao encontre um caractere diferente dos esperados (y,*,x,' ') deve interromper a execucao
*/
int andaEsquerda(pilha_t *pilha){
    int atual = pilha->atual;
    if(infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]-1] == 'y')
        return 2; //achou
    else if (infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]-1] == '*')
        return 0; //limite, nao pode andar mais nesta direcao
    else if (infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]-1] == 'x')
        return -1; //posicao ja caminhada
    else if (infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]-1] == ' '){
        infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]-1] = 'x';
        pilha->linha[atual+1] = pilha->linha[atual];
        pilha->coluna[atual+1] = pilha->coluna[atual]-1;
        pilha->atual++;
        pilha->quantidade++;
        return 1; //sucesso, andou
    }else{
        imprimeMapa();
        printf("simbolo desconhecido no mapa\nsimbolo: '%c'", infoMapa->caminho[pilha->linha[atual]][pilha->coluna[atual]-1]);
        exit(EXIT_FAILURE);
    }
}


/*
Função recebe 80 bits e transforma em um conjunto de 5 mascaras com o seguinte formato:
mascara[0][0-3] 4 direcoes em ordem
mascara[1][0] quantidade de passos a seguir com a mascara [0]
mascara[2][0-3]...
mascara[9][0] quantidade de passos a seguir com a mascara da posicao [8]
*/
void criaMascara(uint8_t *p){
    int i, j, aux=0;
    uint8_t byte;

    for(i=0; i<=8; i+=2, aux=0){
        byte = p[i]; // recebe o byte que vai se transformar nas direcoes
        mascara[i+1][0] = p[i+1]; // recebe byte com a quantidade de passos

        // utiliza dois bits mais significativos para definir primeira direcao (0-3)
        mascara[i][0] = (byte >> 6); 
        // utiliza os 5 bits depois do menos significativo para definir a segunda direcao
        // caso a primeira direcao seja igual a segunda, a segunda eh incrementada em 1
        mascara[i][1] = (mascara[i][0] == (((byte & 62) >> 1) % 3)) ?  (mascara[i][0]+1) : (((byte & 62) >> 1) % 3);
        // caso a segunda direcao se torne 4 por causa do incremento, se torna 0 por definicao
        if(mascara[i][1] == 4)
            mascara[i][1] = 0;

        // aux guarda bits para marcar quais as direcoes ja foram utilizadas
        // caso a direcao 0 ja estiver sendo utilizada, bit menos significativo de aux eh ligado
        // caso a direcao 1 ja estiver sendo utilizada, segundo bit menos significativo de aux eh ligado
        // caso a direcao 2 ja estiver sendo utilizada, terceiro bit menos significativo de aux eh ligado
        // caso a direcao 3 ja estiver sendo utilizada, quarto bit menos significativo de aux eh ligado
        for(j=0; j<2; j++){
            if(mascara[i][j] == 0)
                aux = aux | 1;
            else if(mascara[i][j] == 1)
                aux = aux | 2;
            else if(mascara[i][j] == 2)
                aux = aux | 4;
            else if(mascara[i][j] == 3)
                aux = aux | 8;
        }
        
        // caso algum erro tenha acontecido na geracao da mascara o programa devera ser finalizado
        if(!(aux == 3 || aux == 5 || aux == 6 || aux == 9 || aux == 10 || aux == 12)){
            printf("erro com a mascara");
            exit(EXIT_FAILURE);
        }
        // utiliza o bit menos significativo para definir a terceira direcao
        // se o bit estiver ligado, a maior direcao restante sera a terceira, caso contrario a menor
        // lembrando que a variavel aux guarda as direcoes que ja foram utilizadas
        switch(aux){
            case 3:
                if(byte & 1 == 1){
                    mascara[i][2] = 3;
                    mascara[i][3] = 2;
                }else{
                    mascara[i][2] = 2;
                    mascara[i][3] = 3;
                }
                break;
            case 5:
                if(byte & 1 == 1){
                    mascara[i][2] = 3;
                    mascara[i][3] = 1;
                }else{
                    mascara[i][2] = 1;
                    mascara[i][3] = 3;
                }
                break;
            case 6:
                if(byte & 1 == 1){
                    mascara[i][2] = 3;
                    mascara[i][3] = 0;
                }else{
                    mascara[i][2] = 0;
                    mascara[i][3] = 3;
                }
                break;
            case 9:
                if(byte & 1 == 1){
                    mascara[i][2] = 2;
                    mascara[i][3] = 1;
                }else{
                    mascara[i][2] = 1;
                    mascara[i][3] = 2;
                }
                break;
            case 10:
                if(byte & 1 == 1){
                    mascara[i][2] = 2;
                    mascara[i][3] = 0;
                }else{
                    mascara[i][2] = 0;
                    mascara[i][3] = 2;
                }
                break;
            case 12:
                if(byte & 1 == 1){
                    mascara[i][2] = 1;
                    mascara[i][3] = 0;
                }else{
                    mascara[i][2] = 0;
                    mascara[i][3] = 1;
                }
                break;
        }
    }
}

/* 
funcao retorna numero de passos utilizados para encontrar o ponto final ('y')
*/
float fitness(uint8_t *p){
    int i,qtd;
    pilha_t *pilha;
    FILE *arquivo;

    // carrega arquivo e atualiza limites do mapa (infoMapa->linhas e infoMapa->colunas)
    arquivo = fopen(nomeArquivo, "r");
    infoMapa->linhas=0;
    infoMapa->colunas=0;
    for(i=0;fgets(infoMapa->caminho[i], MAX_COL, arquivo);i++)
        infoMapa->linhas++;
    infoMapa->colunas=strlen(infoMapa->caminho[0]);
    fclose(arquivo);


    if((pilha = (pilha_t *) malloc(sizeof(pilha_t))) == NULL){
        printf("erro na alocacao de memoria para o ponteiro pilha\n");
        exit(EXIT_FAILURE);
    }
    if((pilha->linha = (int *) malloc(MAX_LIN*MAX_COL*sizeof(int))) == NULL){
        printf("erro na alocacao de memoria para o ponteiro pilha->linha\n");
        exit(EXIT_FAILURE);
    } 
    if((pilha->coluna = (int *) malloc(MAX_LIN*MAX_COL*sizeof(int))) == NULL){
        printf("erro na alocacao de memoria para o ponteiro pilha->coluna\n");
        exit(EXIT_FAILURE);
    }
    pilha->atual=0;
    pilha->quantidade=0;

    // neste ponto temos memoria alocada para a pilha inteira
    // ha uma sobra de memoria para os ponteiros linha e coluna pois estou 
    // alocando memoria suficiente para aceitar o maximo de linhas e colunas ao 
    // inves de usar os ponteiros 'linhas' e 'colunas' da estrutura infoMapa 
    // que contem o numero exato de linhas do mapa

    encontraInicio(pilha); // encontra o ponto x do mapa, aonde o 'robo' deve comecar a andar

    criaMascara(p); // cria mascara de direcoes e movimentos com os bytes gerados pelo alg genetico

    percorreMapa(pilha); // percorre mapa e atualiza pilha com a quantidade de movimentos usados

    qtd = pilha->quantidade;

    free(pilha->linha);
    free(pilha->coluna);
    free(pilha);

    return 1./(1+qtd); // retorna resultado da fitness
}

