JFLAGS = -g
JC = javac
BIN_DIR = bin
SRCS = task1/%

check-task%: task%/*.java
	javac -d $(BIN_DIR) $^
	java -cp bin/ ${subst check-,, $@}.Main | sort > tmp1
	java -cp bin/ ${subst check-,, $@}.Sequential | sort > tmp2
	diff tmp1 tmp2 || true
	rm -f tmp1 tmp2

clean:
	rm -rf bin