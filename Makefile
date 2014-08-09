CC = gcc
WFLG = -Wall -Wextra -pedantic -Wunreachable-code -Wformat-security -Wuninitialized -Wimplicit-function-declaration
OBJ = obj/
SRC = src/

pklog: $(OBJ)pklog.o
	$(CC) $< -o $@

$(OBJ)pklog.o: $(SRC)pklog.c
	$(CC) $(WFLG) -c $< -o $@

clean:
	rm pklog $(OBJ)*.o
	
	
