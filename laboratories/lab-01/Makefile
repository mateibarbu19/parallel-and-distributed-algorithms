CC = gcc
CFLAGS = -Wall -Wextra
NUM_THREADS = 4
LOAD_FACTOR = 1
TASKS = task1 task3 task4 task5

build: $(TASKS)

task1: task1.c
	$(CC) $(CFLAGS) -DNUM_THREADS=$(NUM_THREADS) -o $@ $^ -lpthread

task3: task3.c
	$(CC) $(CFLAGS) -DNUM_THREADS=$(NUM_THREADS) -o $@ $^ -lpthread

task4: task4.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

task5: task5.c
	$(CC) $(CFLAGS) -DNUM_THREADS=$(NUM_THREADS) -o $@ $^ -lpthread

weird: weird.c
	$(CC) $(CFLAGS) -D_GNU_SOURCE -DLOAD_FACTOR=$(LOAD_FACTOR) \
	-DNUM_THREADS=$(NUM_THREADS) -o $@ $^ -lpthread

task5-test: task5.c
	$(CC) $(CFLAGS) -DLOAD_FACTOR=$(LOAD_FACTOR) \
	-DNUM_THREADS=$(NUM_THREADS) -o $@ $^ -lpthread

task6:
	./test.sh

clean:
	rm -rf $(TASKS) task5-test weird