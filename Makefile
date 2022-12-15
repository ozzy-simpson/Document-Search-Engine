.PHONY: clean build all

# compilation settings
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -pedantic -std=gnu99 -pedantic -Wstrict-prototypes -Wold-style-definition -Wno-unused-command-line-argument -g 
LDFLAGS= -lm

# directory paths
INCLUDE_DIR = ./
SRC_DIR = ./
OBJ_DIR = ./

# file lists
CFILES = hashmap.c llist.c scores.c search.c
OBJS = hashmap.o llist.o scores.o search.o

# binary
BIN = search

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(BIN) search_scores.txt *~
