CC = mpic++
DEBUG = -ggdb -O0
CFLAGS = -Wall -Wextra # -O3
EXEC = tema3
SRCS := utils.cpp coordinator.cpp process.cpp worker.cpp
HEADERS := $(SRCS:.cpp=.hpp)
SRCS := main.cpp $(SRCS)
N = 9
A = 10
PACK = 333CA_Barbu_Matei.zip

build: $(EXEC)

$(EXEC): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(DEBUG) $(SRCS) -o $(EXEC)

run: $(EXEC) tests
	mpirun --oversubscribe -np $(N) $(EXEC) $(A)

mem-check: $(EXEC) tests
	mpirun --oversubscribe -np $(N) valgrind --leak-check=full --show-leak-kinds=all --log-file=nc.vg.%p ./$(EXEC) $(A)

debug: $(EXEC) tests
	mpirun --oversubscribe -np $(N) konsole -e gdb --args ./$(EXEC) $(A)

tests:
	cp ../tests/test2/cluster* .

$(PACK): $(SRCS) $(HEADERS) Makefile README.md
	zip $@ $^

clean:
	rm -f $(EXEC) nc.vg.* $(PACK) cluster*.txt
