#include "piece.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static const char tipos[PIECE_TYPES_COUNT] = {'I','O','T','L','J','S','Z'};

Peca gerarPeca(int id) {
    Peca p;
    p.id = id;
    /* Usa aleatoriedade dependente de id (srand já inicializado) */
    int idx = rand() % PIECE_TYPES_COUNT;
    p.tipo = tipos[idx];
    return p;
}

void printPeca(Peca p) {
    if (pecaVazia(p)) {
        printf("[ -- ]");
    } else {
        printf("[%02d:%c]", p.id, p.tipo);
    }
}

bool pecaVazia(Peca p) { return p.id < 0; }
