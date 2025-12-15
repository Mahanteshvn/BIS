import random

# Fitness function (minimize)
def fitness(x):
    return x * x

# PSO parameters
NUM_PARTICLES = 20
MAX_ITER = 5          # LIMITED TO 5 ITERATIONS
W = 0.7
C1 = 1.5
C2 = 1.5
LOW, HIGH = -10, 10

# Initialize particles
particles = [random.uniform(LOW, HIGH) for _ in range(NUM_PARTICLES)]
velocities = [random.uniform(-1, 1) for _ in range(NUM_PARTICLES)]

pbest = particles[:]
pbest_fitness = [fitness(x) for x in particles]

gbest = pbest[pbest_fitness.index(min(pbest_fitness))]

# PSO loop (only 5 iterations)
for iteration in range(MAX_ITER):
    for i in range(NUM_PARTICLES):
        r1 = random.random()
        r2 = random.random()

        velocities[i] = (
            W * velocities[i]
            + C1 * r1 * (pbest[i] - particles[i])
            + C2 * r2 * (gbest - particles[i])
        )

        particles[i] += velocities[i]

        # Boundary check
        if particles[i] < LOW:
            particles[i] = LOW
        elif particles[i] > HIGH:
            particles[i] = HIGH

        fit = fitness(particles[i])

        # Update personal best
        if fit < pbest_fitness[i]:
            pbest[i] = particles[i]
            pbest_fitness[i] = fit

    # Update global best
    gbest = pbest[pbest_fitness.index(min(pbest_fitness))]

    print("Iteration", iteration + 1, "Best Solution =", gbest)

print("\nFinal Best Solution:", gbest)
print("Fitness:", fitness(gbest))

