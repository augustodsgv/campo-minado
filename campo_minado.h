/*
Programa : campo_minado.h
Autor: Augustodsgv
Descrição: Jogo campo_minado implementado em c
Data: 23/10/2023
*/
#ifndef CAMPO_MINADO
#define CAMPO_MINADO

#define fieldSize 10                // 18, no vscode parece um tamanho bem justo
#define maximumFieldSize 44         // Há 52 posições horizontais (alfabeto minísculo depois maiúsculo) entretando a formatação acima disso fica zuada
#define nomeArquivo "campo.txt"
#define dificuldade 4       // 0 a 5

const char alfabeto[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// Registro que representa a célula do campo
typedef struct celula{
    int isBomb;
    int isReveald;
    int isMarked;
    int nNeighBombs;
}celula;

// Registro que representa o campo como um todo
typedef struct field{
    celula vetor [fieldSize][fieldSize];
    int nBombas;
    int nReveald;
}field;

// Funções
void printField(field campo);
void printFieldBombs(field campo);
void povoaCampoArquivo(field * campo);
void povoaCampoAleatorio(field * campo);
void findNeighBombs(field * campo);
int letterToInt(char letra);
int getInputOld(int * x, int * y);
int mark(field * campo, int x, int y);
int treatCoord(char * input, int * x, int * y);
int getInput(field * campo);
int firstInput(field * campo);

#endif /*CAMPO_MINADO*/