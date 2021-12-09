CC = gcc
CFLAGS = -Wall -Wextra
DEBUG = -ggdb -O0
LDFLAGS = -lpthread -lm
SOURCES = homework.c genetic_algorithm.c utils.c

build: $(SOURCES)
	@echo "Building..."
	$(CC) $(CFLAGS) -o tema1_par $^ $(LDFLAGS)
	@echo "Done."

build_debug: $(SOURCES)
	@echo "Building..."
	$(CC) $(CFLAGS) $(DEBUG) -o tema1_par $^ $(LDFLAGS)
	@echo "Done."

clean:
	@echo "Cleaning..."
	rm -f tema1_par
	@echo "Done."
