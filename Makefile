# Predmet: IFJ + IAL - FIT VUT Brno                                        
# Projekt: Implementace prekladace imperativniho jazyka IFJ2021
# Vytvoril: Tym 102 - Klimecka Alena, prosinec 2021                            
# Upravy: Tym 102 

OBJECTS = ast.o dynamic_string.o generator.o our_error.o parser.o precedence_parser.o scanner.o structures.o symtab.o

CC = gcc
CFLAGS = -std=c99

.PHONY: program clean

%.o: %.c 
	$(CC) $(CFLAGS) -c $<

all: program

ast.o: ast.c ast.h
dynamic_string.o: dynamic_string.c dynamic_string.h
generator.o: generator.c generator.h
our_error.o: our_error.c our_error.h
parser.o: parser.c parser.h
precedence_parser.o: precedence_parser.c precedence_parser.h
scanner.o: scanner.c scanner.h
structures.o: structures.c structures.h
symtab.o: symtab.c symtab.h

program: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o compiler
	
clean:
	rm -f *.o compiler
