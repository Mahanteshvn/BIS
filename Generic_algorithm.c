#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// Parameters
const int POP_SIZE = 50;
const int NUM_GENERATIONS = 100;
const double CROSSOVER_RATE = 0.8;
const double MUTATION_RATE = 0.1;
const double MIN_X = 0.0;
const double MAX_X = 1.0;
const double TARGET_VALUE = 1.5; // Desired value of f(x)

// Fitness function: minimize error from target
double fitnessFunction(double x) {
    double fx = x * sin(10 * M_PI * x) + 1.0;
    return -abs(fx - TARGET_VALUE); // Higher fitness = closer to target
}

// Generate a random double in range [min, max]
double randomDouble(double min, double max) {
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

// Individual structure
struct Individual {
    double gene;     // x value
    double fitness;

    void evaluate() {
        fitness = fitnessFunction(gene);
    }

    // For sorting (descending fitness)
    bool operator<(const Individual& other) const {
        return fitness > other.fitness;
    }
};

// Selection: Tournament Selection
Individual tournamentSelection(const vector<Individual>& population) {
    int a = rand() % POP_SIZE;
    int b = rand() % POP_SIZE;
    return (population[a].fitness > population[b].fitness) ? population[a] : population[b];
}

// Crossover: Arithmetic crossover
pair<Individual, Individual> crossover(const Individual& p1, const Individual& p2) {
    Individual c1 = p1, c2 = p2;
    if (randomDouble(0, 1) < CROSSOVER_RATE) {
        double alpha = randomDouble(0, 1);
        c1.gene = alpha * p1.gene + (1 - alpha) * p2.gene;
        c2.gene = alpha * p2.gene + (1 - alpha) * p1.gene;
    }
    return {c1, c2};
}

// Mutation: Small random change
void mutate(Individual& ind) {
    if (randomDouble(0, 1) < MUTATION_RATE) {
        double mutation_strength = 0.1;
        ind.gene += randomDouble(-mutation_strength, mutation_strength);
        // Clamp gene to valid range
        if (ind.gene < MIN_X) ind.gene = MIN_X;
        if (ind.gene > MAX_X) ind.gene = MAX_X;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Step 1: Initialize population
    vector<Individual> population(POP_SIZE);
    for (auto& ind : population) {
        ind.gene = randomDouble(MIN_X, MAX_X);
        ind.evaluate();
    }

    Individual best_individual = population[0];

    // Step 2: Evolution loop
    for (int gen = 0; gen < NUM_GENERATIONS; ++gen) {
        vector<Individual> new_population;

        while (new_population.size() < POP_SIZE) {
            // Selection
            Individual parent1 = tournamentSelection(population);
            Individual parent2 = tournamentSelection(population);

            // Crossover
            auto [child1, child2] = crossover(parent1, parent2);

            // Mutation
            mutate(child1);
            mutate(child2);

            // Evaluation
            child1.evaluate();
            child2.evaluate();

            // Add to new population
            new_population.push_back(child1);
            if (new_population.size() < POP_SIZE)
                new_population.push_back(child2);
        }

        // Replace old population
        population = new_population;

        // Track best individual
        for (auto& ind : population) {
            if (ind.fitness > best_individual.fitness) {
                best_individual = ind;
            }
        }

        // Print best fitness of the generation
        cout << "Generation " << gen + 1
             << " - Closest Fitness: " << best_individual.fitness
             << " (Error: " << abs(best_individual.fitnessFunction(best_individual.gene) - TARGET_VALUE) << ")" << endl;
    }

    // Output the best solution
    double final_fx = best_individual.gene * sin(10 * M_PI * best_individual.gene) + 1.0;
    cout << "\nClosest Solution Found:\n";
    cout << "x = " << best_individual.gene
         << ", f(x) = " << final_fx
         << ", Error = " << abs(final_fx - TARGET_VALUE) << endl;

    return 0;
}
