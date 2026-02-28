CC = gcc
CFLAGS = -Wall -Wextra
SRC = main.c scanner.c
OUT = portscanner

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
