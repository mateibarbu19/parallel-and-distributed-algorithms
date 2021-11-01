CC = gcc
CFLAGS = -Wall -Wextra -ggdb -O0
LDFLAGS = -lpthread -lm
TARGETS = oets shear merge parallel_binary_search

build: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS)