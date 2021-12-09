#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "individual.h"
#include "sack_object.h"
#include "utils.h"

typedef struct thread_data_t {
  unsigned int thread_id;
  size_t thread_count;
  const sack_object *objects;
  int object_count;
  int generations_count;
  int sack_capacity;
  pthread_barrier_t *barrier;
  individual *current_generation, *next_generation, *tmp_generation;
} thread_data;

// reads input from a given file
int read_input(sack_object **objects, int *object_count, int *sack_capacity,
               int *generations_count, int argc, char *argv[]);

// displays all the objects that can be placed in the sack
void print_objects(const sack_object *objects, int object_count);

// displays all or a part of the individuals in a generation
void print_generation(const individual *generation, int limit);

// displays the individual with the best fitness in a generation
void print_best_fitness(const individual *generation);

// computes the characteristics for each individual in a generation
void compute_characteristics(const sack_object *objects,
                              individual *generation, int object_count,
                              int sack_capacity, unsigned int thread_id,
                              unsigned int thread_count);

// compares two individuals by fitness and then number of objects in the sack
// (to be used with qsort)
int cmp_individuals(const void *a, const void *b);

// performs a variant of bit string mutation
void mutate_bit_string_1(const individual *ind, int generation_index);

// performs a different variant of bit string mutation
void mutate_bit_string_2(const individual *ind, int generation_index);

// performs one-point crossover
void crossover(individual *parent1, individual *child1, int generation_index);

// copies one individual
void copy_individual(const individual *from, const individual *to);

// initiates a individual generation
void init_generation(individual *generation, size_t limit,
                     unsigned int thread_id, unsigned int thread_count);

// deallocates a generation
void free_generation(individual *generation, unsigned int thread_id,
                     unsigned int thread_count);

// runs the genetic algorithm
void *run_genetic_algorithm(void *arg);

#endif