# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 $(shell sdl2-config --cflags) $(shell pkg-config --cflags SDL2_image) $(shell pkg-config --cflags SDL2_ttf)
LDFLAGS = $(shell sdl2-config --libs) $(shell pkg-config --libs SDL2_image) $(shell pkg-config --libs SDL2_ttf) -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-network

# Source files and output executable
SRCS = client.c client_hracie_pole.c client_hrac.c
TARGET = client

# Object files
OBJS = $(SRCS:.c=.o) 

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
