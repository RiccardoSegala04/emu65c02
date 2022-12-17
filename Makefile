CC = gcc

CFLAGS = -g -Wall 
INCLUDE = include
SOURCE = src
LIBRARIES = -lncurses 
BIN = rel
TARGET = emu
DEST = /usr/local/bin
RM = rm

default: build

build:
	[ -d $(BIN) ] || mkdir -p $(BIN)
	$(CC) $(CFLAGS) -I $(INCLUDE) -c src/*.c
	$(CC) *.o $(LIBRARIES) -o $(TARGET)
	mv *.o $(BIN)
	mv $(TARGET) $(BIN)

debug:
	[ -d $(BIN) ] || mkdir -p $(BIN)
	$(CC) $(CFLAGS) -I $(INCLUDE) -D__LOG_ENABLE -D__LOG_DEBUG -D__LOG_FILE -c src/*.c
	$(CC) *.o $(LIBRARIES) -o $(TARGET)
	mv *.o $(BIN)
	mv $(TARGET) $(BIN)


run:
	./$(BIN)/$(TARGET)

clean:
	$(RM) -r $(BIN)

install:
	cp ./$(BIN)/$(TARGET) $(DEST)

remove:
	$(RM) $(DEST)/$(TARGET)
