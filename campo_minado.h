/*
Programa : campo_minado.h
Autor: Augustodsgv
Descrição: Jogo campo_minado implementado em c
Data: 23/10/2023
*/
#ifndef CAMPO_MINADO
#define CAMPO_MINADO

// #define fieldSize 10                // 18, no vscode parece um tamanho bem justo
#define alfabetoSize 52
#define maximumFieldSize 44         // Há 52 posições horizontais (alfabeto minísculo depois maiúsculo) entretando a formatação acima disso fica zuada
#define nomeArquivo "campo.txt"
#define dificuldade 4       // 0 a 5

const char alfabeto[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// Registro que representa a célula do campo
typedef struct celula{
    int isBomb;
    int isOpened;
    int isMarked;
    int nNeighBombs;
}celula;

// Registro que representa o campo como um todo
typedef struct field{
    celula ** vetor;
    int nBombas;
    int nReveald;
    int fieldSize;
}field;

// Funções
void printField(field campo);
void printFieldBombs(field campo);
void povoaCampoArquivo(field * campo, int fieldSize);
void povoaCampoAleatorio(field * campo, int fieldSize, int dificulty);
void findNeighBombs(field * campo);
int letterToInt(char letra);
int getInputOld(field * campo, int * x, int * y);
void mark(field * campo, int x, int y);
void unmark(field * campo, int x, int y);
int treatCoord(field * campo, char * input, int * x, int * y);
int getInput(field * campo);
int firstInput(field * campo);

int openRecDepth(field * campo, int x, int y);

#endif /*CAMPO_MINADO*/