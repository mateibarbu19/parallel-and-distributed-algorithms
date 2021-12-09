#include "genetic_algorithm.h"
#include "individual.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_input(sack_object **objects, int *object_count, int *sack_capacity,
               int *generations_count, int argc, char *argv[]) {
  FILE *fp;

  if (argc != 4) {
    fprintf(stderr,
            "Usage:\n\t./tema1_par in_file generations_count thread_count\n");
    return 0;
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    return 0;
  }

  int P = atoi(argv[3]);
  if (P < 1) {
    return 0;
  }
  unsigned int thread_count = (unsigned int)P;

  if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
    fclose(fp);
    return 0;
  }

  if (*object_count % 10) {
    fclose(fp);
    return 0;
  }

  sack_object *tmp_objects =
      (sack_object *)calloc(*object_count, sizeof(sack_object));

  for (int i = 0; i < *object_count; ++i) {
    if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) <
        2) {
      free(objects);
      fclose(fp);
      return 0;
    }
  }

  fclose(fp);

  *generations_count = (int)strtol(argv[2], NULL, 10);

  if (*generations_count == 0) {
    free(tmp_objects);

    return 0;
  }

  *objects = tmp_objects;

  return thread_count;
}

void print_objects(const sack_object *objects, int object_count) {
  for (int i = 0; i < object_count; ++i) {
    printf("%d %d\n", objects[i].weight, objects[i].profit);
  }
}

void print_generation(const individual *generation, int limit) {
  for (int i = 0; i < limit; ++i) {
    for (int j = 0; j < generation[i].chromosome_length; ++j) {
      printf("%d ", generation[i].chromosomes[j]);
    }

    printf("\n%d - %d - %d - %d - %d\n", i, generation[i].fitness,
           generation[i].chromosome_length, generation[i].index,
           generation[i].count);
  }
}

void print_best_fitness(const individual *generation) {
  printf("%d\n", generation[0].fitness);
}

void compute_characteristics(const sack_object *objects,
                              individual *generation, int object_count,
                              int sack_capacity, unsigned int thread_id,
                              unsigned int thread_count) {
  int weight;
  int profit;

  int start = thread_id * object_count / thread_count;
  int end = (thread_id + 1) * object_count / thread_count;
  for (int i = start; i < end; ++i) {
    weight = 0;
    profit = 0;
    generation[i].count = 0;

    for (int j = 0; j < generation[i].chromosome_length; ++j) {
      if (generation[i].chromosomes[j]) {
        weight += objects[j].weight;
        profit += objects[j].profit;
        generation[i].count++;
      }
    }

    generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
  }
}

int cmp_individuals(const void *a, const void *b) {
  individual *first = (individual *)a;
  individual *second = (individual *)b;

  int res = second->fitness - first->fitness; // decreasing by fitness
  if (res == 0) {
    res = first->count -
          second->count; // increasing by number of objects in the sack
    if (res == 0) {
      return second->index - first->index;
    }
  }

  return res;
}

void mutate_bit_string_1(const individual *ind, int generation_index) {
  int i, mutation_size;
  int step = 1 + generation_index % (ind->chromosome_length - 2);

  if (ind->index % 2 == 0) {
    // for even-indexed individuals, mutate the first 40% chromosomes by a given
    // step
    mutation_size = ind->chromosome_length * 4 / 10;
    for (i = 0; i < mutation_size; i += step) {
      ind->chromosomes[i] = 1 - ind->chromosomes[i];
    }
  } else {
    // for even-indexed individuals, mutate the last 80% chromosomes by a given
    // step
    mutation_size = ind->chromosome_length * 8 / 10;
    for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length;
         i += step) {
      ind->chromosomes[i] = 1 - ind->chromosomes[i];
    }
  }
}

void mutate_bit_string_2(const individual *ind, int generation_index) {
  int step = 1 + generation_index % (ind->chromosome_length - 2);

  // mutate all chromosomes by a given step
  for (int i = 0; i < ind->chromosome_length; i += step) {
    ind->chromosomes[i] = 1 - ind->chromosomes[i];
  }
}

void crossover(individual *parent1, individual *child1, int generation_index) {
  individual *parent2 = parent1 + 1;
  individual *child2 = child1 + 1;
  int count = 1 + generation_index % parent1->chromosome_length;

  memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
  memcpy(child1->chromosomes + count, parent2->chromosomes + count,
         (parent1->chromosome_length - count) * sizeof(int));

  memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
  memcpy(child2->chromosomes + count, parent1->chromosomes + count,
         (parent1->chromosome_length - count) * sizeof(int));
}

void copy_individual(const individual *from, const individual *to) {
  memcpy(to->chromosomes, from->chromosomes,
         from->chromosome_length * sizeof(int));
}

void swap_individual(const void *a, const void *b) {
  individual *first = (individual *)a;
  individual *second = (individual *)b;
  swap_val(first->fitness, second->fitness);
  swap_val(first->chromosomes, second->chromosomes);
  swap_val(first->chromosome_length, second->chromosome_length);
  swap_val(first->index, second->index);
  swap_val(first->count, second->count);
}

void free_generation(individual *generation, unsigned int thread_id,
                     unsigned int thread_count) {
  int i;

  int start = thread_id * generation->chromosome_length / thread_count;
  int end = (thread_id + 1) * generation->chromosome_length / thread_count;
  for (i = start; i < end; ++i) {
    free(generation[i].chromosomes);
    generation[i].chromosomes = NULL;
    generation[i].fitness = 0;
  }
}

void init_generation(individual *generation, size_t limit,
                     unsigned int thread_id, unsigned int thread_count) {
  unsigned int start = thread_id * limit / thread_count;
  unsigned int end = (thread_id + 1) * limit / thread_count;
  for (unsigned int i = start; i < end; ++i) {
    generation[i].fitness = 0;
    generation[i].chromosomes = (int *)calloc(limit, sizeof(int));
    generation[i].chromosomes[i] = 1;
    generation[i].index = i;
    generation[i].chromosome_length = limit;
    generation[i].count = 0;
  }
}

void *run_genetic_algorithm(void *arg) {
  unsigned thread_id = ((thread_data *)arg)->thread_id;
  size_t thread_count = ((thread_data *)arg)->thread_count;
  const sack_object *objects = ((thread_data *)arg)->objects;
  int object_count = ((thread_data *)arg)->object_count;
  int generations_count = ((thread_data *)arg)->generations_count;
  int sack_capacity = ((thread_data *)arg)->sack_capacity;
  pthread_barrier_t *barrier = ((thread_data *)arg)->barrier;
  individual *current_generation = ((thread_data *)arg)->current_generation;
  individual *next_generation = ((thread_data *)arg)->next_generation;
  individual *tmp_generation = ((thread_data *)arg)->tmp_generation;

  int count, cursor;

  individual *tmp = NULL;

  // set initial generation (composed of object_count individuals with a single
  // item in the sack)
  init_generation(current_generation, object_count, thread_id, thread_count);
  init_generation(next_generation, object_count, thread_id, thread_count);
  init_generation(tmp_generation, object_count, thread_id, thread_count);

  // iterate for each generation
  for (int k = 0; k < generations_count; ++k) {
    cursor = 0;

    // compute fitness and sort by it
    compute_characteristics(objects, current_generation, object_count,
                             sack_capacity, thread_id, thread_count);
    p_mergesort(current_generation, tmp_generation, sizeof(individual),
                object_count, cmp_individuals, swap_individual, thread_id,
                thread_count, barrier);

    // keep first 30% children (elite children selection)
    count = object_count * 3 / 10;
    int start = thread_id * count / thread_count;
    int end = (thread_id + 1) * count / thread_count;
    for (int i = start; i < end; ++i) {
      copy_individual(current_generation + i, next_generation + i);
    }
    cursor = count;

    // mutate first 20% children with the first version of bit string mutation
    count = object_count * 2 / 10;
    start = thread_id * count / thread_count;
    end = (thread_id + 1) * count / thread_count;
    for (int i = start; i < end; ++i) {
      copy_individual(current_generation + i, next_generation + cursor + i);
      mutate_bit_string_1(next_generation + cursor + i, k);
    }
    cursor += count;

    // mutate next 20% children with the second version of bit string mutation
    count = object_count * 2 / 10;
    start = thread_id * count / thread_count;
    end = (thread_id + 1) * count / thread_count;
    for (int i = start; i < end; ++i) {
      copy_individual(current_generation + i + count,
                      next_generation + cursor + i);
      mutate_bit_string_2(next_generation + cursor + i, k);
    }
    cursor += count;

    // crossover first 30% parents with one-point crossover
    // (if there is an odd number of parents, the last one is kept as such)
    count = object_count * 3 / 10;

    if ((count & 1) == 1) {
      if (thread_id == 0) {
        copy_individual(current_generation + object_count - 1,
                        next_generation + cursor + count - 1);
      }
      count--;
    }

    start = make_even(thread_id * count / thread_count);
    end = make_even((thread_id + 1) * count / thread_count);
    for (int i = start; i < end; i += 2) {
      crossover(current_generation + i, next_generation + cursor + i, k);
    }

    pthread_barrier_wait(barrier);

    // switch to new generation
    tmp = current_generation;
    current_generation = next_generation;
    next_generation = tmp;

    start = thread_id * object_count / thread_count;
    end = (thread_id + 1) * object_count / thread_count;
    for (int i = start; i < end; ++i) {
      current_generation[i].index = i;
    }

    if (thread_id == 0 && k % 5 == 0) {
      print_best_fitness(current_generation);
    }
  }

  compute_characteristics(objects, current_generation, object_count,
                           sack_capacity, thread_id, thread_count);
  p_mergesort(current_generation, tmp_generation, sizeof(individual),
              object_count, cmp_individuals, swap_individual, thread_id,
              thread_count, barrier);
  if (thread_id == 0) {
    print_best_fitness(current_generation);
  }

  // free resources for old generation
  free_generation(current_generation, thread_id, thread_count);
  free_generation(next_generation, thread_id, thread_count);
  free_generation(tmp_generation, thread_id, thread_count);

  return NULL;
}