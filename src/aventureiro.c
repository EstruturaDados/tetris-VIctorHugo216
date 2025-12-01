#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "piece.h"

#define QUEUE_CAPACITY 5
#define STACK_CAPACITY 3

typedef struct Fila {
    Peca data[QUEUE_CAPACITY];
    int front;
    int size;
    int nextId;
} Fila;

typedef struct Pilha {
    Peca data[STACK_CAPACITY];
    int top; /* índice do próximo slot livre (0 quando vazia) */
    int size;
} Pilha;

void inicializarFila(Fila *f) {
    f->front = 0; f->size = 0; f->nextId = 1;
    for (int i = 0; i < QUEUE_CAPACITY; ++i) f->data[i].id = -1;
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        Peca p = gerarPeca(f->nextId++);
        int idx = (f->front + f->size) % QUEUE_CAPACITY;
        f->data[idx] = p; f->size++;
    }
}

void inicializarPilha(Pilha *s) {
    s->top = 0; s->size = 0;
    for (int i = 0; i < STACK_CAPACITY; ++i) s->data[i].id = -1;
}

int filaVazia(Fila *f) { return f->size == 0; }
int filaCheia(Fila *f) { return f->size == QUEUE_CAPACITY; }
int pilhaVazia(Pilha *s) { return s->size == 0; }
int pilhaCheia(Pilha *s) { return s->size == STACK_CAPACITY; }

Peca dequeue(Fila *f) {
    Peca p; p.id = -1;
    if (filaVazia(f)) return p;
    p = f->data[f->front];
    f->data[f->front].id = -1;
    f->front = (f->front + 1) % QUEUE_CAPACITY;
    f->size--;
    return p;
}

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return -1;
    int idx = (f->front + f->size) % QUEUE_CAPACITY;
    f->data[idx] = p; f->size++; return 0;
}

int push(Pilha *s, Peca p) {
    if (pilhaCheia(s)) return -1;
    s->data[s->top++] = p; s->size++; return 0;
}

Peca pop(Pilha *s) {
    Peca p; p.id = -1;
    if (pilhaVazia(s)) return p;
    p = s->data[--s->top]; s->data[s->top].id = -1; s->size--;
    return p;
}

void mostrarFila(Fila *f) {
    printf("Fila atual (front=%d size=%d):\n", f->front, f->size);
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        int idx = (f->front + i) % QUEUE_CAPACITY;
        printf(" %d: ", i+1); printPeca(f->data[idx]); printf(i<QUEUE_CAPACITY-1?"  ":"\n");
    }
}

void mostrarPilha(Pilha *s) {
    printf("Pilha de reserva (top=%d size=%d): ", s->top, s->size);
    if (pilhaVazia(s)) { printf("(vazia)\n"); return; }
    for (int i = s->top - 1; i >= 0; --i) {
        printPeca(s->data[i]); if (i>0) printf(" ");
    }
    printf("\n");
}

int main() {
    srand((unsigned)time(NULL));

    Fila fila; Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    printf("=== Tetris Stack - Nível Aventureiro ===\n");

    int opt = -1;
    while (1) {
        printf("\nMenu:\n1 - Jogar peça (dequeue)\n2 - Reservar peça (push)\n3 - Usar peça reservada (pop)\n4 - Mostrar estado\n0 - Sair\nEscolha: ");
        if (scanf("%d", &opt) != 1) break;
        if (opt == 0) break;

        if (opt == 1) {
            if (filaVazia(&fila)) {
                printf("Fila vazia — nada a jogar.\n");
            } else {
                Peca jogada = dequeue(&fila);
                printf("Jogou a peça: "); printPeca(jogada); printf("\n");
                Peca nova = gerarPeca(fila.nextId++);
                enqueue(&fila, nova);
                printf("Nova adicionada: "); printPeca(nova); printf("\n");
            }
        } else if (opt == 2) {
            if (pilhaCheia(&pilha)) {
                printf("Pilha cheia — não é possível reservar mais peças.\n");
            } else if (filaVazia(&fila)) {
                printf("Fila vazia — nada para reservar.\n");
            } else {
                Peca front = dequeue(&fila);
                if (push(&pilha, front) == 0) {
                    printf("Reservou peça: "); printPeca(front); printf("\n");
                    Peca nova = gerarPeca(fila.nextId++);
                    enqueue(&fila, nova);
                    printf("Nova adicionada: "); printPeca(nova); printf("\n");
                } else {
                    printf("Erro ao reservar (push).\n");
                }
            }
        } else if (opt == 3) {
            if (pilhaVazia(&pilha)) {
                printf("Pilha vazia — sem peças reservadas.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("Usou peça reservada: "); printPeca(usada); printf("\n");
            }
        } else if (opt == 4) {
            mostrarFila(&fila); mostrarPilha(&pilha);
        } else {
            printf("Opção inválida.\n");
        }

        /* mostra fila e pilha após cada ação válida */
        if (opt >= 1 && opt <= 3) {
            printf("\nEstado atualizado:\n"); mostrarFila(&fila); mostrarPilha(&pilha);
        }
    }

    printf("Saindo...\n");
    return 0;
}
