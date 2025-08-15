# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I./include -O2 -fPIC
LDFLAGS = -lmicrohttpd

# Output names
LIB = libmathlib.a
SERVER = server

# Source files
LIB_SRCS = src/mathlib.c
LIB_OBJS = mathlib.o

SERVER_SRCS = src/server.c
SERVER_OBJS = server.o

# Default target
all: $(LIB) $(SERVER)

# Build static library
$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

# Compile object files
mathlib.o: src/mathlib.c
	$(CC) $(CFLAGS) -c src/mathlib.c -o mathlib.o

server.o: src/server.c
	$(CC) $(CFLAGS) -c src/server.c -o server.o

# Link server executable
$(SERVER): server.o $(LIB)
	$(CC) $(CFLAGS) -o $(SERVER) server.o $(LIB) $(LDFLAGS)

# Clean build files
clean:
	rm -f *.o $(LIB) $(SERVER)
