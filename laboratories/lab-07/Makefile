JFLAGS = -g
JC = javac
BIN_DIR = bin
SRCS = task1/%

$(BIN_DIR)/task%/Main.class: task%/*
	javac -d $(BIN_DIR) $^

clean:
	rm -rf bin