CC = g++
CFLAGS = -Wall -Wextra -ggdb -O0
LDFLAGS = -lpthread
TARGETS = test_1_old test_1_new

build: $(TARGETS)

%: %.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGETS)
