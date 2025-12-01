#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "piece.h"

#define QUEUE_CAPACITY 5
#define STACK_CAPACITY 3

typedef struct Fila {
    Peca data[QUEUE_CAPACITY];
    int front; int size; int nextId;
} Fila;

typedef struct Pilha {
    Peca data[STACK_CAPACITY];
    int top; int size;
} Pilha;

typedef struct Snapshot {
    Fila fila;
    Pilha pilha;
    int valid;
} Snapshot;

void salvarSnapshot(Snapshot *s, Fila *f, Pilha *p) {
    s->fila = *f;
    s->pilha = *p;
    s->valid = 1;
}

void restaurarSnapshot(Snapshot *s, Fila *f, Pilha *p) {
    if (!s->valid) return;
    *f = s->fila;
    *p = s->pilha;
    s->valid = 0;
}

void inicializarFila(Fila *f) {
    f->front = 0; f->size = 0; f->nextId = 1;
    for (int i=0;i<QUEUE_CAPACITY;++i) f->data[i].id = -1;
    for (int i=0;i<QUEUE_CAPACITY;++i) { Peca p = gerarPeca(f->nextId++); int idx = (f->front + f->size) % QUEUE_CAPACITY; f->data[idx]=p; f->size++; }
}

void inicializarPilha(Pilha *s) { s->top=0; s->size=0; for (int i=0;i<STACK_CAPACITY;++i) s->data[i].id=-1; }

int filaVazia(Fila *f) { return f->size==0; }
int filaCheia(Fila *f) { return f->size==QUEUE_CAPACITY; }
int pilhaVazia(Pilha *p) { return p->size==0; }
int pilhaCheia(Pilha *p) { return p->size==STACK_CAPACITY; }

Peca dequeue(Fila *f) {
    Peca p; p.id = -1; if (filaVazia(f)) return p; p = f->data[f->front]; f->data[f->front].id=-1; f->front = (f->front + 1) % QUEUE_CAPACITY; f->size--; return p;
}

int enqueue(Fila *f, Peca p) { if (filaCheia(f)) return -1; int idx = (f->front + f->size) % QUEUE_CAPACITY; f->data[idx]=p; f->size++; return 0; }

int push(Pilha *s, Peca p) { if (pilhaCheia(s)) return -1; s->data[s->top++] = p; s->size++; return 0; }

Peca pop(Pilha *s) { Peca p; p.id=-1; if (pilhaVazia(s)) return p; p = s->data[--s->top]; s->data[s->top].id=-1; s->size--; return p; }

void mostrarFila(Fila *f) {
    printf("Fila atual (front=%d size=%d):\n", f->front, f->size);
    for (int i=0;i<QUEUE_CAPACITY;++i) {
        int idx=(f->front + i) % QUEUE_CAPACITY;
        printf(" %d: ", i+1); printPeca(f->data[idx]); printf(i<QUEUE_CAPACITY-1?"  ":"\n");
    }
}

void mostrarPilha(Pilha *p) {
    printf("Pilha (top=%d size=%d): ", p->top, p->size);
    if (pilhaVazia(p)) { printf("(vazia)\n"); return; }
    for (int i=p->top-1;i>=0;--i) { printPeca(p->data[i]); if (i>0) printf(" "); }
    printf("\n");
}

int swapTopPilhaComFront(Fila *f, Pilha *p) {
    if (filaVazia(f)) return -1;
    if (pilhaVazia(p)) return -2;
    /* troca front com top-1 */
    int pf = f->front;
    int ps = p->top - 1;
    Peca tmp = f->data[pf]; f->data[pf] = p->data[ps]; p->data[ps] = tmp; return 0;
}

int swapThree(Fila *f, Pilha *p) {
    if (p->size != 3) return -1;
    if (f->size < 3) return -2;
    /* mapeia a pilha de cima para baixo para os 3 primeiros da fila (frente..frente+2) */
    for (int i = 0; i < 3; ++i) {
        int queueIdx = (f->front + i) % QUEUE_CAPACITY;
        int stackIdx = p->top - 1 - i; /* pilha de cima para baixo */
        Peca tmp = f->data[queueIdx]; f->data[queueIdx] = p->data[stackIdx]; p->data[stackIdx] = tmp;
    }
    return 0;
}

int main() {
    srand((unsigned)time(NULL));
    Fila fila; Pilha pilha; Snapshot last; last.valid = 0;
    inicializarFila(&fila); inicializarPilha(&pilha);

    printf("=== Tetris Stack - Nível Mestre ===\n");
    int opt = -1;

    while (1) {
        printf("\nMenu:\n1 - Jogar peça\n2 - Reservar peça\n3 - Usar peça reservada\n4 - Trocar topo da pilha com frente da fila\n5 - Desfazer última jogada\n6 - Trocar 3 primeiros da fila com 3 da pilha\n0 - Sair\nEscolha: ");
        if (scanf("%d", &opt) != 1) break;
        if (opt == 0) break;

        if (opt == 1) {
            salvarSnapshot(&last, &fila, &pilha);
            if (filaVazia(&fila)) { printf("Fila vazia — nada a jogar.\n"); }
            else {
                Peca jogada = dequeue(&fila);
                printf("Jogou: "); printPeca(jogada); printf("\n");
                Peca nova = gerarPeca(fila.nextId++); enqueue(&fila, nova);
                printf("Nova peça adicionada: "); printPeca(nova); printf("\n");
            }
        } else if (opt == 2) {
            salvarSnapshot(&last, &fila, &pilha);
            if (pilhaCheia(&pilha)) { printf("Pilha cheia — não é possível reservar.\n"); }
            else if (filaVazia(&fila)) { printf("Fila vazia — nada para reservar.\n"); }
            else {
                Peca front = dequeue(&fila);
                if (push(&pilha, front) == 0) {
                    printf("Reservou: "); printPeca(front); printf("\n");
                    Peca nova = gerarPeca(fila.nextId++); enqueue(&fila, nova); printf("Nova adicionada: ");printPeca(nova); printf("\n");
                }
            }
        } else if (opt == 3) {
            salvarSnapshot(&last, &fila, &pilha);
            if (pilhaVazia(&pilha)) { printf("Pilha vazia — sem peças reservadas.\n"); }
            else { Peca usada = pop(&pilha); printf("Usou peça reservada: "); printPeca(usada); printf("\n"); }
        } else if (opt == 4) {
            salvarSnapshot(&last, &fila, &pilha);
            int r = swapTopPilhaComFront(&fila, &pilha);
            if (r == -1) printf("Fila vazia — operação inválida.\n"); else if (r == -2) printf("Pilha vazia — operação inválida.\n"); else printf("Troca realizada entre topo da pilha e frente da fila.\n");
        } else if (opt == 5) {
            if (!last.valid) { printf("Nada para desfazer.\n"); }
            else { restaurarSnapshot(&last, &fila, &pilha); printf("Última ação desfeita.\n"); }
        } else if (opt == 6) {
            salvarSnapshot(&last, &fila, &pilha);
            int r = swapThree(&fila, &pilha);
            if (r == -1) printf("Pilha não tem exatamente 3 peças.\n"); else if (r == -2) printf("Fila precisa ter pelo menos 3 peças.\n"); else printf("Troca de 3 peças realizada com sucesso.\n");
        } else {
            printf("Opção inválida.\n");
        }

        printf("\nEstado atual:\n"); mostrarFila(&fila); mostrarPilha(&pilha);
    }

    printf("Saindo...\n");
    return 0;
}
