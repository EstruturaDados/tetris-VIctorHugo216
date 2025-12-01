#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>

#define PIECE_TYPES_COUNT 7

typedef struct Peca {
    char tipo; /* 'I','O','T','L','J','S','Z' */
    int id;    /* identificador único */
} Peca;

/* cria uma nova peça com id fornecido */
Peca gerarPeca(int id);

/* imprime uma peça no formato: [id:tipo] */
void printPeca(Peca p);

/* compara se peça é vazia (id < 0 usado como sentinela) */
bool pecaVazia(Peca p);

#endif /* PIECE_H */
