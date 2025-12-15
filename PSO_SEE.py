import random

# -----------------------------
# Fitness function (minimize)
# -----------------------------
def fitness(position):
    x = position[0]
    y = position[1]
    # Example: cost function
    return (x - 3)**2 + (y + 5)**2

# -----------------------------
# PSO parameters
# -----------------------------
NUM_PARTICLES = 10      # number of particles
NUM_DIMENSIONS = 2      # x and y
MAX_ITER = 5            # LIMIT TO 5 ITERATIONS
W = 0.7                 # inertia weight
C1 = 1.5                # cognitive coefficient
C2 = 1.5                # social coefficient
LOW, HIGH = -10, 10     # bounds for x and y

# -----------------------------
# Initialize particles
# -----------------------------
particles = [[random.uniform(LOW, HIGH) for _ in range(NUM_DIMENSIONS)] for _ in range(NUM_PARTICLES)]
velocities = [[random.uniform(-1, 1) for _ in range(NUM_DIMENSIONS)] for _ in range(NUM_PARTICLES)]

pbest = [p[:] for p in particles]                    # personal best positions
pbest_fitness = [fitness(p) for p in particles]     # personal best fitness

gbest_index = pbest_fitness.index(min(pbest_fitness))
gbest = pbest[gbest_index][:]                       # global best position

# -----------------------------
# PSO main loop (5 iterations)
# -----------------------------
for iteration in range(MAX_ITER):
    for i in range(NUM_PARTICLES):
        for d in range(NUM_DIMENSIONS):
            r1 = random.random()
            r2 = random.random()
            velocities[i][d] = (W * velocities[i][d]
                                + C1 * r1 * (pbest[i][d] - particles[i][d])
                                + C2 * r2 * (gbest[d] - particles[i][d]))
            particles[i][d] += velocities[i][d]
            # Keep within bounds
            if particles[i][d] < LOW:
                particles[i][d] = LOW
            elif particles[i][d] > HIGH:
                particles[i][d] = HIGH

        # Evaluate fitness
        fit = fitness(particles[i])

        # Update personal best
        if fit < pbest_fitness[i]:
            pbest[i] = particles[i][:]
            pbest_fitness[i] = fit

    # Update global best
    gbest_index = pbest_fitness.index(min(pbest_fitness))
    gbest = pbest[gbest_index][:]
    best_fit = pbest_fitness[gbest_index]

    print(f"Iteration {iteration+1}, Best Fitness = {best_fit:.6f}, Best Position = {gbest}")

# -----------------------------
# Final result
# -----------------------------
print("\nFinal Best Solution:", gbest)
print("Final Best Fitness:", best_fit)
