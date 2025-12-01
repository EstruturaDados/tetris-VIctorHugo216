#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "piece.h"

#define QUEUE_CAPACITY 5

typedef struct Fila {
    Peca data[QUEUE_CAPACITY];
    int front;   /* índice do primeiro elemento */
    int size;    /* número de elementos */
    int nextId;  /* próximo id para gerar novas peças */
} Fila;

void inicializarFila(Fila *f) {
    f->front = 0;
    f->size = 0;
    f->nextId = 1;
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        f->data[i].id = -1; /* marca como vazio */
    }
    /* preenche a fila com 5 peças iniciais */
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        Peca p = gerarPeca(f->nextId++);
        int idx = (f->front + f->size) % QUEUE_CAPACITY;
        f->data[idx] = p;
        f->size++;
    }
}

int filaVazia(Fila *f) { return f->size == 0; }
int filaCheia(Fila *f) { return f->size == QUEUE_CAPACITY; }

/* remove da frente e retorna peça removida. O chamador deve tratar o caso vazio. */
Peca dequeue(Fila *f) {
    if (filaVazia(f)) {
        Peca p; p.id = -1; return p;
    }
    Peca out = f->data[f->front];
    f->data[f->front].id = -1; /* marca como removido */
    f->front = (f->front + 1) % QUEUE_CAPACITY;
    f->size--;
    return out;
}

/* adiciona no final. retorna 0 em sucesso, -1 se cheia */
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return -1;
    int idx = (f->front + f->size) % QUEUE_CAPACITY;
    f->data[idx] = p;
    f->size++;
    return 0;
}

void mostrarFila(Fila *f) {
    printf("Fila atual (front idx=%d, size=%d):\n", f->front, f->size);
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        int idx = (f->front + i) % QUEUE_CAPACITY;
        printf(" %d: ", i+1);
        printPeca(f->data[idx]);
        if (i < QUEUE_CAPACITY-1) printf("  ");
    }
    printf("\n");
}

int main() {
    srand((unsigned)time(NULL));
    Fila fila;
    inicializarFila(&fila);

    printf("===== Tetris Stack - Nível Novato =====\n");

    int opt = -1;
    while (1) {
        printf("\nMenu:\n1 - Jogar peça (dequeue)\n2 - Mostrar fila\n0 - Sair\nEscolha: ");
        if (scanf("%d", &opt) != 1) break;
        if (opt == 0) break;

        if (opt == 1) {
            if (filaVazia(&fila)) {
                printf("Fila vazia — nada a jogar.\n");
            } else {
                Peca jogada = dequeue(&fila);
                printf("Jogou a peça: "); printPeca(jogada); printf("\n");
                /* reposição automática */
                Peca nova = gerarPeca(fila.nextId++);
                if (enqueue(&fila, nova) != 0) {
                    /* Isso não deve ocorrer: após dequeue há sempre espaço na fila */
                    printf("Erro: não foi possível inserir nova peça.\n");
                } else {
                    printf("Nova peça adicionada ao final: "); printPeca(nova); printf("\n");
                }
            }
            mostrarFila(&fila);
        } else if (opt == 2) {
            mostrarFila(&fila);
        } else {
            printf("Opção inválida.\n");
        }
    }

    printf("Saindo...\n");
    return 0;
}
