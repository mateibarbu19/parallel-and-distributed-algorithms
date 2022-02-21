CC = mpicc
CFLAGS = -Wall -Wextra
EXEC = test_2

build: $(EXEC)

$(EXEC): %: %.c
	$(CC) $(CFLAGS) $^ -o $@

run: $(EXEC)
	mpirun --oversubscribe -np 12 $(EXEC)

clean:
	rm -f $(EXEC)
