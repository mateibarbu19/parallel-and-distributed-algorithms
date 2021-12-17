CC = mpicc
CFLAGS = 
EXEC =

build: $(EXEC)

$(EXEC): %: %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(EXEC)
