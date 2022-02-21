CC = gcc
NUM_THREADS = 4
CFLAGS = -Wall -Wextra -O0
LDFLAGS = -lpthread
PFLAGS = -DNUM_THREADS=$(NUM_THREADS)
PROTECTED=
TARGETS = mutex barrier multiply_seq multiply_outer multiply_middle \
		  multiply_inner strassen strassen_par

build: $(TARGETS)

% : %.c
	$(CC) $(CFLAGS) $(PROTECTED) $(PFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS)
	rm -f seq.txt par_outer.txt par_middle.txt par_inner.txt par.txt
