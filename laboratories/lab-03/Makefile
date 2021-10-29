CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread -lm
TARGETS = oets shear merge parallel_binary_search

build: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS)