CC = gcc
CFLAGS = -Wall
TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)


clean:
	rm -f $(TARGET)
