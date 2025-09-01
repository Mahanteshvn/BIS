#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POP_SIZE 50
#define NUM_GENERATIONS 100
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.1
#define MIN_X 0.0
#define MAX_X 1.0
#define TARGET_VALUE 1.5

// Individual structure
typedef struct {
    double gene;
    double fitness;
} Individual;

// Fitness function: minimize error from target
double fitnessFunction(double x) {
    double fx = x * sin(10 * M_PI * x) + 1.0;
    return -fabs(fx - TARGET_VALUE); // Higher fitness = closer to target
}

// Generate a random double in range [min, max]
double randomDouble(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Evaluate fitness
void evaluate(Individual* ind) {
    ind->fitness = fitnessFunction(ind->gene);
}

// Tournament selection
Individual tournamentSelection(Individual population[]) {
    int a = rand() % POP_SIZE;
    int b = rand() % POP_SIZE;
    return (population[a].fitness > population[b].fitness) ? population[a] : population[b];
}

// Crossover: Arithmetic crossover
void crossover(Individual* p1, Individual* p2, Individual* c1, Individual* c2) {
    *c1 = *p1;
    *c2 = *p2;
    if (randomDouble(0, 1) < CROSSOVER_RATE) {
        double alpha = randomDouble(0, 1);
        c1->gene = alpha * p1->gene + (1 - alpha) * p2->gene;
        c2->gene = alpha * p2->gene + (1 - alpha) * p1->gene;
    }
}

// Mutation
void mutate(Individual* ind) {
    if (randomDouble(0, 1) < MUTATION_RATE) {
        double mutation_strength = 0.1;
        ind->gene += randomDouble(-mutation_strength, mutation_strength);
        if (ind->gene < MIN_X) ind->gene = MIN_X;
        if (ind->gene > MAX_X) ind->gene = MAX_X;
    }
}

int main() {
    srand((unsigned)time(NULL));

    Individual population[POP_SIZE];
    Individual new_population[POP_SIZE];
    Individual best_individual;

    // Initialize population
    for (int i = 0; i < POP_SIZE; ++i) {
        population[i].gene = randomDouble(MIN_X, MAX_X);
        evaluate(&population[i]);
    }
    best_individual = population[0];

    // Evolution loop
    for (int gen = 0; gen < NUM_GENERATIONS; ++gen) {
        int count = 0;
        while (count < POP_SIZE) {
            Individual parent1 = tournamentSelection(population);
            Individual parent2 = tournamentSelection(population);

            Individual child1, child2;
            crossover(&parent1, &parent2, &child1, &child2);

            mutate(&child1);
            mutate(&child2);

            evaluate(&child1);
            evaluate(&child2);

            new_population[count++] = child1;
            if (count < POP_SIZE) {
                new_population[count++] = child2;
            }
        }

        // Replace old population
        for (int i = 0; i < POP_SIZE; ++i) {
            population[i] = new_population[i];
            if (population[i].fitness > best_individual.fitness) {
                best_individual = population[i];
            }
        }

        double error = fabs(best_individual.gene * sin(10 * M_PI * best_individual.gene) + 1.0 - TARGET_VALUE);
        printf("Generation %d - Closest Fitness: %.6f (Error: %.6f)\n", gen + 1, best_individual.fitness, error);
    }

    double final_fx = best_individual.gene * sin(10 * M_PI * best_individual.gene) + 1.0;
    double final_error = fabs(final_fx - TARGET_VALUE);
    printf("\nClosest Solution Found:\n");
    printf("x = %.6f, f(x) = %.6f, Error = %.6f\n", best_individual.gene, final_fx, final_error);

    return 0;
}
