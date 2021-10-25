CC = gcc
NUM_THREADS = 4
CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread
PFLAGS = -DNUM_THREADS=$(NUM_THREADS)
PROTECTED=
TARGETS = mutex barrier multiply_seq multiply_outer multiply_middle \
		  multiply_inner strassen strassen_par

build: $(TARGETS)

% : %.c
	$(CC) $(CFLAGS) $(PROTECTED) $(PFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGETS)
