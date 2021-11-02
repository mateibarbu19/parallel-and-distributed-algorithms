CC = gcc
CFLAGS = -Wall -Wextra -ggdb -O0
LDFLAGS = -pthread -lm
TARGETS = oets shear merge parallel_search

build: $(TARGETS)

%-scalar: %.c
	$(CC) $(CFLAGS) -DSCALABILITY -o $@ $^ $(LDFLAGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *-scalar