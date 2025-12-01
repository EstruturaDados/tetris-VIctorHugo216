CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRC_DIR = src

all: novato aventureiro mestre

novato: $(SRC_DIR)/novato.c $(SRC_DIR)/piece.c
	$(CC) $(CFLAGS) $(SRC_DIR)/novato.c $(SRC_DIR)/piece.c -o novato

aventureiro: $(SRC_DIR)/aventureiro.c $(SRC_DIR)/piece.c
	$(CC) $(CFLAGS) $(SRC_DIR)/aventureiro.c $(SRC_DIR)/piece.c -o aventureiro

mestre: $(SRC_DIR)/mestre.c $(SRC_DIR)/piece.c
	$(CC) $(CFLAGS) $(SRC_DIR)/mestre.c $(SRC_DIR)/piece.c -o mestre

clean:
	rm -f novato aventureiro mestre

.PHONY: all clean
