/*
Programa : campo_minado.c
Autor: Augustodsgv
Descrição: Jogo campo_minado implementado em c
Data: 23/10/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "campo_minado.h"


// Função que printa o campo de maneira formatada
void printField(field campo){
    printf("    ");
    for(int j = 1; j < campo.fieldSize + 1; j++)          // Fazendo as coordenadas horizontais com números
        printf("  %d ", j);
    printf("\n");
    for (int i = 0; i < campo.fieldSize; i++){
        printf("    ");
        for(int j = 0; j < campo.fieldSize; j++)
            printf("+---");
        printf("+\n");
        printf(" %c  ", alfabeto[i]);               // Coordenadas verticais com letras

        for(int j = 0; j < campo.fieldSize; j++){         // Printando o campo
            celula celulaAtual = campo.vetor[i][j];
            // Caso esteja oculta
            if (!celulaAtual.isReveald){
                // Caso esteja limpa
                if (!celulaAtual.isMarked)
                    printf("| . ");
                else    // Caso esteja marcada
                    printf("| # ");
            }else{      // Caso a celula esteja exibida
                // Caso não seja uma bomba, será exibido o número de vizinhos
                if (!celulaAtual.isBomb)
                    printf("| %d ", celulaAtual.nNeighBombs);
                else        // Caso seja uma bomba
                    printf("| ò ");
            }
        }
        printf("|\n");
    }
    printf("    ");
    for(int j = 0; j < campo.fieldSize; j++){
        printf("+---");
    };
    printf("+\n");
}

// Função que printa as bombas no campo
void printFieldBombs(field campo){
    printf("    ");
    for(int j = 1; j < campo.fieldSize + 1; j++){         // Fazendo as coordenadas horizontais com números
        if (j < 10)
            printf("  %d ", j);
        else
            printf(" %d ", j);

    }
    printf("\n");
    for (int i = 0; i < campo.fieldSize; i++){
        printf("    ");
        for(int j = 0; j < campo.fieldSize; j++)
            printf("+---");
        printf("+\n");
        printf(" %c  ", alfabeto[i]);               // Coordenadas verticais com letras

        for(int j = 0; j < campo.fieldSize; j++){         // Printando o campo
            celula celulaAtual = campo.vetor[i][j];
            if (!celulaAtual.isBomb)
                printf("|   ");
            else        // Caso seja uma bomba
                printf("| ò ");
            
        }
        printf("|\n");
    }
    printf("    ");
    for(int j = 0; j < campo.fieldSize; j++){
        printf("+---");
    };
    printf("+\n");
}

// Função que preenche o campo com as bombinhas e espaços vagos
/*  Formato:
0 1 0 1
1 0 1 0
0 1 0 1
1 0 0 1

0 representa células livres, 1 bombas.
As colunas são separadas por espaço, as linhas por \n
*/
void povoaCampoArquivo(field * campo, int fieldSize){
    // Inicializando parâmetros do campo
    campo->nBombas = 0;
    campo->nReveald = 0;
    campo->fieldSize = fieldSize;

    char campoAtual;
    FILE * arquivo = fopen(nomeArquivo, "r");
    for (int i = 0; i < campo->fieldSize; i++){
        for(int j = 0; j < campo->fieldSize; j++){
            // Eliminando o s '\n' e ' '
            do{
                fread(&campoAtual, sizeof(char), 1, arquivo);
            }while(campoAtual != '0' && campoAtual != '1');

            campo->vetor[i][j].nNeighBombs = 0;                        // Iniciando com 0 pois não se sabe ainda quantos vizinhos há
            campo->vetor[i][j].isBomb = (int)campoAtual - 48;          // Em ascii, '0' é 48 e '1' é 49
            campo->vetor[i][j].isReveald = 0;
            campo->vetor[i][j].isMarked = 0;            
        }
    }
}

// Função que cria um campo com bombas espalhadas de forma aleatória
// Está sendo usado uma proporção de 2 célula livres para 1 bomba, pois 50% 50% parecia muito difícil
// void povoaCampoAleatorio(field * campo, int dificuldade){
void povoaCampoAleatorio(field * campo, int fieldSize){


    // Inicializando parâmetros do campo
    campo->nBombas = 0;
    campo->nReveald = 0;
    campo->fieldSize = fieldSize;

    // Alocando o campo
    campo->vetor = (celula**)malloc(sizeof(celula *) * fieldSize);
    for(int i = 0; i < fieldSize; i++){
        campo->vetor[i] = (celula*) malloc (sizeof(celula) * fieldSize);
    }

    // Gerando os números "aleatórios"
    srand(time(NULL));

    // Povoando o campo
    for(int i = 0; i < campo->fieldSize; i++)
        for(int j = 0; j < campo->fieldSize; j++){
            campo->vetor[i][j].isBomb = !(rand() % (7 - dificuldade));                 // O resto da divisão de 7, será ou 0 ou um número inteiro. Dessa forma, quando maior for o divisor, men
            campo->vetor[i][j].nNeighBombs = 0;                     // Iniciando com 0 pois não se sabe ainda quantos vizinhos há
            campo->vetor[i][j].isReveald = 0;
            campo->vetor[i][j].isMarked = 0;
        }
    
}

// Função que calcula quantos bombas há na vizinhança de uma célula
void findNeighBombs(field * campo){
    for (int i = 0; i < campo->fieldSize; i++){        // Loop pela matriz
        for (int j = 0; j < campo->fieldSize; j++){
            // Aqui usaremos uma abordagem de, quando acharmos uma bomba, adicionamos 1 à quantidade de bombas vizinhas
            if (campo->vetor[i][j].isBomb){
                campo->nBombas++;       // Adicionando ao contador de bombas  
                // Andando na matriz 3x3 dos vizinhos da célula 
                for(int k = i - 1; k <= i + 1; k++){     // Controle vertical, indo de i - 1 (linha de cima) até i + 1 (linha de baixo)
                    // Verificando se a linha não está estourando para cima ou para baixo
                    if (k >= 0 && k < campo->fieldSize){
                        for(int l = j - 1; l <= j + 1; l++){         // Controle horizontal, indo de j - i (coluna do lado esquerdo) até j + 1 (coluna do lado direito)
                            // Verificando se a coluna não está estourando para a esquerda ou a direita
                            if (l >= 0 && l < campo->fieldSize){
                                // Verificando se não é a mesma célula
                                if(!(k == i && l == j))
                                    campo->vetor[k][l].nNeighBombs++;
                            }
                        }
                    }
                }

            }
        }
    }
}

// Converte um input em caractere (alfabeto de a - z e A - Z para um número das coordenadas inteiras da matriz)
int letterToInt(char letra){
    for(int i = 0; i < alfabetoSize; i++)
        if(letra == alfabeto[i])
            return i;
    return -1;
}

// Função que convert string de números inteiros para int
/*
    (int)str[i] converte o valor para ascii
    Os números em ascii são de 0 a 9, sendo o 0 com numero 48, 1 : 49 ... 9 : 57
    Como os números virão na ordem ['9', '5', '7'], a casa mais significativa é a primeira e tem um expoente maior
    Dessa maneira, precisamos multiplicá-la pela base 10 que tem no número inteiro. "957", como no exemplo
    é 9 * 100, 5 * 10, 7 * 1
    Para saber a ordem de grandeza do maior dígito, podemos usar o tamanho da string. strlen('957') = 3
    Assim, sabemos que o expoente vai variar de strlen('957') - 1 até 0
    */
int strToInt(char * str){
    int inteiro = 0;
    int recuo = 1;          // Essa variável controla o número de símbolos marcadores de fim. Toda string termina com \0,
                            // Entretanto strlen já não conta isso, logo o 1 é somente por que o vetor vai de 0 até len - 1
    if (str[strlen(str) - 2] == '\n')       // Verificando se termina com \n
        recuo++;
        
    printf("tamanho da str %d\n", strlen(str));
    for (int i = 0;i < strlen(str); i--){
        inteiro += ((int)str[i] - 48) * pow(10, (strlen(str) - recuo) - i);
    }

    printf("Convertendo %s para %d\n", str, inteiro);
    return inteiro;
}

// Retorna se o input é válido e as coordenadas do input do usuário por referência
int getInputOld(field * campo, int * x, int * y){
    char yChar;
    printf("Insira suas coordenadas: ");
    
    scanf("%c", &yChar);
    scanf("%d", x);

    while (getchar() != '\n');      // Removendo o '\n'

    *y = letterToInt(yChar);        // Convertendo o input em letra da coordenada y para inteiro

    // Checando o input y
    if(*y == -1){
        printf("Coordenada de letra inválida. Tente uma letra entre \"a\" e \"%c\"\n", alfabeto[campo->fieldSize]);
        return 0;
    }

    if (*x < 1 || *x > campo->fieldSize){
        printf("Coordenada numeral inválida. Tente um número entre 1 e %d\n", campo->fieldSize);
        return 0;
    }

    *x = *x - 1;       // o input x inicia-se em 1 e não em 0, como no vetor

    return 1;   // Caso válido
}

void mark(field * campo, int x, int y){
    campo->vetor[y][x].isMarked = 1;                // X E Y SÃO INVERTIDOS!!!
}

void unmark(field * campo, int x, int y){
    campo->vetor[y][x].isMarked = 0;                // X E Y SÃO INVERTIDOS!!!
}

int reveal(field * campo, int x, int y){
    campo->vetor[y][x].isReveald = 1;
    campo->nReveald++;

    // Verificando o caso de bombas
    if(campo->vetor[y][x].isBomb){
        printf("fim de jogo: achou uma bomba!!\n");
        printFieldBombs(*campo);
        exit(1);
    }
    if (campo->nBombas == campo->nReveald){
        printf("Parabéns!!! Você encontrou todas as %d casas vazias!\n");
        exit(1);
    }
}

// Função que pega o input e retorna, por referência, as coordenadas x e y
int treatCoord(field * campo, char * input, int * x, int * y){
    char yChar;
    char * xChar;

    // Tratando o input da coordenada horizontal (y)
    yChar = strtok(NULL, " ")[0];           // Pegando o primeiro termo da string, pois o próximo é um \0

    *y = letterToInt(yChar);
    // Checando o input y
    if(*y == -1){
        printf("Coordenada de letra inválida. Tente uma letra entre \"a\" e \"%c\"\n", alfabeto[campo->fieldSize]);
        return 0;
    }

    // Tratando o input da coordenada vertical (x)
    // Aqui precisaremos tratar cada dígito do número individualmente, pois agora se trata de uma string e não de um inteiro mais
    xChar = strtok(NULL, " ");

    *x = strToInt(xChar);          
    if (*x < 1 || *x > campo->fieldSize){
        printf("Coordenada numeral %d inválida. Tente um número entre 1 e %d\n", *x, campo->fieldSize);
        return 0;
    }

    *x = *x - 1;       // o input x inicia-se em 1 e não em 0, como no vetor
    return 1;
}

// O comando tem um formato de "?????"
// [comando][arg1][arg2][arg3]
// Lista de comandos:
/*
(TODO) START [fieldSize][dificulty]        -> Inicia um novo jogo com um tamanho de campo e dificuldade definidas
(TODO) FINISH                              -> Finaliza o jogo atual e mostra as bombas
OPEN [coordX][coordY]                      -> Abre a revela designada.
O[coordX][coordY]                          -> Comando OPEN
MARK [coordX][coordY]                      -> Faz uma marcação na célula designada
M [coordX][coordY]                         -> Comando MARK
*/

int getInput(field * campo){
    int x, y;
    char input[20];
    char * comando;        // O comando tem a sequência [comando][coordX][coordY]
    fgets(input, 20, stdin);
    comando = strtok(input, " ");

    if (!strcmp(comando, "MARK") || !strcmp(comando, "m")){
        if (treatCoord(campo, input, &x, &y)){
            mark(campo, x, y);
            return 1;
            }
        return 0;
    }

    if (!strcmp(comando, "UNMARK") || !strcmp(comando, "u")){
        if (treatCoord(campo, input, &x, &y)){
            unmark(campo, x, y);
            return 1;
            }
        return 0;
    }

    if (!strcmp(comando, "OPEN") || !strcmp(comando, "o")){
        if (treatCoord(campo, input, &x, &y))
            return reveal(campo, x, y);
        return 0;
    }
    /*
    if (!strcmp(comando, "FINISH")){
        printf("Fim de jogo: jogador desistiu\n");
        exit(0);
    }
    */
   printf("Comando inválido, tente novamente\n");
   return 0;
}


int firstInput(field * campo){
    int x, y;
    int xMin, xMax, yMin, yMax;         // Variáveis referentes à área ao redor do clique inicial

    char input[20];
    char * comando;        // O comando tem a sequência [comando][coordX][coordY]
    fgets(input, 20, stdin);
    comando = strtok(input, " ");

    if (!strcmp(comando, "MARK") || !strcmp(comando, "m")){
        if (treatCoord(campo, input, &x, &y))
            mark(campo, x, y);
        return 0;
    }

    if (!strcmp(comando, "UNMARK") || !strcmp(comando, "u")){
        if (treatCoord(campo, input, &x, &y))
            unmark(campo, x, y);
        return 0;
    }

    if (!strcmp(comando, "OPEN") || !strcmp(comando, "o")){
        if (treatCoord(campo, input, &x, &y)){
            // Calculando as coordenadas horizontais
            if (y < 1){                // Caso a área ao redor do p. inicial esteja acima da área do campo
                yMin = y;
                yMax = y + 2;
            }else{                      
                if (y == campo->fieldSize - 1){      // Caso a área ao redor do p. inicial esteja abaiyo da área do campo
                    yMin = y - 2;
                    yMax = y;
                }else{                   // Caso esteja totalmente dentro da área do campo  
                    yMin = y - 1;
                    yMax = y + 1;
                }
            }

            // Calculando as coordenadas verticais
            if (x < 1){                // Caso a área ao redor do p. inicial esteja à esquerdo da área do campo
                xMin = x;
                xMax = x + 2;
            }else{                      
                if (x == campo->fieldSize - 1){      // Caso a área ao redor do p. inicial esteja à direita da área do campo
                    xMin = x - 2;
                    xMax = x;
                }else{                   // Caso esteja totalmente dentro da área do campo  
                    xMin = x - 1;
                    xMax = x + 1;
                }
            }
            // printf("reveladas as coordenadas x entre %d e %d e y entre %d e %d\n", xMin, xMax, yMin, yMax);
            campo->vetor[x][y].isBomb = 0;           // Transformando o ponto clicado numa célula livre no hard-code e reve

            findNeighBombs(campo);                  // Fazendo a contagem das bomba depois de determinar o ponto inicial, visto que se este foi uma bomba muda a contagem

            for (int i = yMin; i <= yMax; i++)
                for (int j = xMin; j <= xMax; j++){
                    if(!campo->vetor[i][j].isBomb){      // Só revela se for livre
                        campo->vetor[i][j].isReveald = 1;
                        campo->nReveald++;
                    }
                }
            return 1;
        }
    }
    printf("Comando inválido, tente novamente\n");
    return 0;
}


int main(int argc, char *argv[]){
    field campo;
    int fieldSize;

    if (argc > 1){
        fieldSize = strToInt(argv[1]);
    }else{
        fieldSize = 10;
    }

    povoaCampoAleatorio(&campo, fieldSize);
    
    do{
        printField(campo);
    }while(!firstInput(&campo));    

    while(1){
        printf("Faltam %d bombas a serem encontradas\n", campo.nBombas - campo.nReveald);
        printField(campo);

        while(!getInput(&campo));
    }
    
    return 0;
}