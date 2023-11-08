CC = gcc
CFLAGS = -Wall -pthread -g
SRC = main.c report.c producer.c consumer.c queue.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = tradecrypto

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECUTABLE) $(OBJ)

.PHONY: all clean

