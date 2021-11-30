JFLAGS = -g
JC = javac
BIN_DIR = bin
SRCS = task1/%

build: task1/*
	

task%: task%/*
	javac -d $(BIN_DIR) $<

check-task%: task%
	java -cp bin/ $<.Main | sort > tmp1
	java -cp bin/ $<.Sequential | sort > tmp2
	$(shell diff tmp1 tmp2)
	rm -f tmp1 tmp2

clean:
	rm -rf bin