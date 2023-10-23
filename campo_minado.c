#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define fieldSize 10                // 18, no vscode parece um tamanho bem justo
#define maximumFieldSize 44         // Há 52 posições horizontais (alfabeto minísculo depois maiúsculo) entretando a formatação acima disso fica zuada
#define nomeArquivo "campo.txt"
#define dificuldade 4       // 0 a 5

const char alfabeto[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int passo = 0;
typedef struct celula{
    int isBomb;
    int isReveald;
    int isMarked;
    int nNeighBombs;
}celula;

typedef struct field{
    celula vetor [fieldSize][fieldSize];
    int nBombas;
    int nReveald;
}field;

// Função que printa o campo de maneira formatada
void printField(field campo){
    printf("    ");
    for(int j = 1; j < fieldSize + 1; j++)          // Fazendo as coordenadas horizontais com números
        printf("  %d ", j);
    printf("\n");
    for (int i = 0; i < fieldSize; i++){
        printf("    ");
        for(int j = 0; j < fieldSize; j++)
            printf("+---");
        printf("+\n");
        printf(" %c  ", alfabeto[i]);               // Coordenadas verticais com letras

        for(int j = 0; j < fieldSize; j++){         // Printando o campo
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
    for(int j = 0; j < fieldSize; j++){
        printf("+---");
    };
    printf("+\n");
}

// Função que printa as bombas no campo
void printFieldBombs(field campo){
    printf("    ");
    for(int j = 1; j < fieldSize + 1; j++){         // Fazendo as coordenadas horizontais com números
        if (j < 10)
            printf("  %d ", j);
        else
            printf(" %d ", j);

    }
    printf("\n");
    for (int i = 0; i < fieldSize; i++){
        printf("    ");
        for(int j = 0; j < fieldSize; j++)
            printf("+---");
        printf("+\n");
        printf(" %c  ", alfabeto[i]);               // Coordenadas verticais com letras

        for(int j = 0; j < fieldSize; j++){         // Printando o campo
            celula celulaAtual = campo.vetor[i][j];
            if (!celulaAtual.isBomb)
                printf("|   ");
            else        // Caso seja uma bomba
                printf("| ò ");
            
        }
        printf("|\n");
    }
    printf("    ");
    for(int j = 0; j < fieldSize; j++){
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
void povoaCampoArquivo(field * campo){
    // Inicializando parâmetros do campo
    campo->nBombas = 0;
    campo->nReveald = 0;

    char campoAtual;
    FILE * arquivo = fopen(nomeArquivo, "r");
    for (int i = 0; i < fieldSize; i++){
        for(int j = 0; j < fieldSize; j++){
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
void povoaCampoAleatorio(field * campo){
    // Inicializando parâmetros do campo
    campo->nBombas = 0;
    campo->nReveald = 0;

    // Gerando os números "aleatórios"
    srand(time(NULL));

    // Povoando o campo
    for(int i = 0; i < fieldSize; i++)
        for(int j = 0; j < fieldSize; j++){
            campo->vetor[i][j].isBomb = !(rand() % (7 - dificuldade));                 // O resto da divisão de 7, será ou 0 ou um número inteiro. Dessa forma, quando maior for o divisor, men
            campo->vetor[i][j].nNeighBombs = 0;                     // Iniciando com 0 pois não se sabe ainda quantos vizinhos há
            campo->vetor[i][j].isReveald = 0;
            campo->vetor[i][j].isMarked = 0;
        }
}


// Função que calcula quantos bombas há na vizinhança de uma célula
void findNeighBombs(field * campo){
    for (int i = 0; i < fieldSize; i++){        // Loop pela matriz
        for (int j = 0; j < fieldSize; j++){
            // Aqui usaremos uma abordagem de, quando acharmos uma bomba, adicionamos 1 à quantidade de bombas vizinhas
            if (campo->vetor[i][j].isBomb){
                campo->nBombas++;       // Adicionando ao contador de bombas  
                // Andando na matriz 3x3 dos vizinhos da célula 
                for(int k = i - 1; k <= i + 1; k++){     // Controle vertical, indo de i - 1 (linha de cima) até i + 1 (linha de baixo)
                    // Verificando se a linha não está estourando para cima ou para baixo
                    if (k >= 0 && k < fieldSize){
                        for(int l = j - 1; l <= j + 1; l++){         // Controle horizontal, indo de j - i (coluna do lado esquerdo) até j + 1 (coluna do lado direito)
                            // Verificando se a coluna não está estourando para a esquerda ou a direita
                            if (l >= 0 && l < fieldSize){
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
    for(int i = 0; i < fieldSize; i++)
        if(letra == alfabeto[i])
            return i;
    return -1;
}

// Retorna se o input é válido e as coordenadas do input do usuário por referência
int getInputOld(int * x, int * y){
    char yChar;
    printf("Insira suas coordenadas: ");
    
    scanf("%c", &yChar);
    scanf("%d", x);

    while (getchar() != '\n');      // Removendo o '\n'

    *y = letterToInt(yChar);        // Convertendo o input em letra da coordenada y para inteiro

    // Checando o input y
    if(*y == -1){
        printf("Coordenada de letra inválida. Tente uma letra entre \"a\" e \"%c\"\n", alfabeto[fieldSize]);
        return 0;
    }

    if (*x < 1 || *x > fieldSize){
        printf("Coordenada numeral inválida. Tente um número entre 1 e %d\n", fieldSize);
        return 0;
    }

    *x = *x - 1;       // o input x inicia-se em 1 e não em 0, como no vetor

    return 1;   // Caso válido
}

int mark(field * campo, int x, int y){
    campo->vetor[y][x].isMarked = 1;                // X E Y SÃO INVERTIDOS!!!
    return 1;
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
int treatCoord(char * input, int * x, int * y){
    char yChar;
    char * xChar;

    // Tratando o input da coordenada horizontal (y)
    yChar = strtok(NULL, " ")[0];           // Pegando o primeiro termo da string, pois o próximo é um \0

    *y = letterToInt(yChar);
    // Checando o input y
    if(*y == -1){
        printf("Coordenada de letra inválida. Tente uma letra entre \"a\" e \"%c\"\n", alfabeto[fieldSize]);
        return 0;
    }

    // Tratando o input da coordenada vertical (x)
    // Aqui precisaremos tratar cada dígito do número individualmente, pois agora se trata de uma string e não de um inteiro mais
    xChar = strtok(NULL, " ");

    if (strlen(xChar) < 3){                      // Caso haja somente 1 dígito, ou seja, < 10
       *x = ((int)xChar[0] - 48);
    }else{                                      // Caso seja maior que 10, o primeiro dígito é das dezenas e o segundo das unidades
        printf("entrou aqui");
        *x = ((int)xChar[0] - 48) * 10 + ((int)xChar[1] - 48);
    }            
    if (*x < 1 || *x > fieldSize){
        printf("Coordenada numeral %d inválida. Tente um número entre 1 e %d\n", *x, fieldSize);
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
    char input[20];
    char * comando;        // O comando tem a sequência [comando][coordX][coordY]
    fgets(input, 20, stdin);
    comando = strtok(input, " ");

    if (!strcmp(comando, "MARK") || !strcmp(comando, "m")){
        int x, y;
        if (treatCoord(input, &x, &y))
            return mark(campo, x, y);
        return 0;
    }

    if (!strcmp(comando, "OPEN") || !strcmp(comando, "o")){
        int x, y;
        if (treatCoord(input, &x, &y))
            return reveal(campo, x, y);
        return 0;
    }
    /*
    if (!strcmp(comando, "FINISH")){
        printf("Fim de jogo: jogador desistiu\n");
        exit(0);
    }
    */
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
        if (treatCoord(input, &x, &y))
            mark(campo, x, y);
        return 0;
    }

    if (!strcmp(comando, "OPEN") || !strcmp(comando, "o")){
        if (treatCoord(input, &x, &y)){
            // Calculando as coordenadas horizontais
            if (y < 1){                // Caso a área ao redor do p. inicial esteja acima da área do campo
                yMin = y;
                yMax = y + 2;
            }else{                      
                if (y == fieldSize - 1){      // Caso a área ao redor do p. inicial esteja abaiyo da área do campo
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
                if (x == fieldSize - 1){      // Caso a área ao redor do p. inicial esteja à direita da área do campo
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
                    printf("chegando na célula %d %d\n", i, j);

                    if(!campo->vetor[i][j].isBomb){      // Só revela se for livre
                        printf("revelando a célula %d %d\n", i, j);
                        campo->vetor[i][j].isReveald = 1;
                        campo->nReveald++;
                    }
                }
            }
        }
    return 1;
    
}



int main(){
    field campo;
    int x, y;

    // povoaCampoArquivo(&campo);
    povoaCampoAleatorio(&campo);
    
    do{
        printField(campo);
    }while(!firstInput(&campo));
    // /*

    // O primeiro input deve ser livre de bombas, e liberar uma área ao redor do primeiro ponto.
    // Por hora essa área será de 3x3

    /*
    +---+---+---+---+---+---+---+---+---+---+
    |   |   | ò |   | ò |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   | ò |   |   |   |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   | ò | ò | # | # | # |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   | ò |   | # | X | # |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   |   | ò | # | # | # |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   | ò |   |   |   |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   |   |   |   | ò |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   | ò | ò |   |   |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    |   | ò |   | ò |   |   |   | ò |   | ò |
    +---+---+---+---+---+---+---+---+---+---+
    */    

    while(1){
        printf("Faltam %d bombas a serem encontradas\n", campo.nBombas - campo.nReveald);
        printField(campo);

        while(!getInput(&campo));
    }
    
    return 0;
}