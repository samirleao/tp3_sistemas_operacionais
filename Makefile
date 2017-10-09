# Makefile para o tp3 de Sistemas Operacionais
# Simulador de memoir virtual
# Sera gerado um executavel de nome tp3virtual
# make clean para apagar arquivos objeto
# Aluno: Samir Moreira Andrade Leao

CC  = gcc
OBJ = main.o algoritmos.o virtual.o

all: tp3

tp3: $(OBJ)
	$(CC) -o tp3virtual $(OBJ)

algoritmos.o: algoritmos.c virtual.h
	$(CC) -c algoritmos.c

virtual.o: virtual.c virtual.h
	$(CC) -c virtual.c
	
main.o: main.c virtual.h
	$(CC) -c main.c

clean:
	rm *.o
