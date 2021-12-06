CC = mpicc
CFLAGS = 
OBJS = example mpi_hello scatter_gather send_recv_array

build: $(OBJS)

$(OBJS): %: %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)
