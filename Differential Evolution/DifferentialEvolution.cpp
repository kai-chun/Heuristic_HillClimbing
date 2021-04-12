#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <iomanip> 
#include <algorithm>
using namespace std;

double PI = 3.141592653589793116;

// experience environment
int iteration = 1000;
int exp_time = 10;
int population_size = 10;

// setting of Sphere equation
double s_domain_min = -100;
double s_domain_max = 100;
int s_dim = 2;

// setting of Rastrigin equation
double r_domain_min = -1.5;
double r_domain_max = 1.5;
int r_dim = 2;

// input of Sphere equation
double SphereEqu(double x[s_dim]){
    double result = 0;

    for (int i = 0; i < s_dim; i++){
        // check if x is out of domain
        if (x[i] < s_domain_min){
            x[i] = s_domain_min;
        }
        if (x[i] > s_domain_max){
            x[i] = s_domain_max;
        }
        result += pow(x[i], 2);
    }
    
    return result;
}

// input of Rastrigin equation
double RastriginEqu(double x[r_dim]){
    double result = 10 * r_dim;

    for (int i = 0; i < r_dim; i++){
        // check if x is out of domain
        if (x[i] < r_domain_min){
            x[i] = r_domain_min;
        }
        if (x[i] > r_domain_max){
            x[i] = r_domain_max;
        }
        result += (pow(x[i], 2) - 10 * cos(2 * PI * x[i]));
    }

    return result;
}

// randomly selected vectors from population list
double randomVector(int a, int b){
    double r = (double) rand() / (RAND_MAX + 1.0);

    int c = 0 + (int) (r * (population_size - 0));
    while (c == a || c == b){
        r = (double) rand() / (RAND_MAX + 1.0);
        c = 0 + (int) (r * (population_size - 0));
    }

    return c;
}

// Differential Evolution for Sphere equation
void DifferentialEvolution_S(int time){
    double targetVector[population_size][s_dim];
    double mutantVector[population_size][s_dim];
    double trialVector[population_size][s_dim];

    double scalingFactor = 1.2;

    int q; // guarantees at least one component from mutant vector
    double controlParameter = 0.8; // control diversity of the population

    double min_fitness = s_domain_max;
    int min_iteration = 0;

    double bestVector[s_dim]; // record the best target vector in the last iteration

    // Initialization: generate random seed
    for (int i = 0; i < population_size; i++){
        for (int j = 0; j < s_dim; j++){
            targetVector[i][j] = (s_domain_max - s_domain_min) * rand() / (RAND_MAX + 1.0) + s_domain_min;
        }
    }

    // iteration begin
    for (int i = 0; i < iteration; i++){

        // Mutation
        for (int j = 0; j < population_size; j++){
            // random select vector
            int b = randomVector(j, -1);
            int c = randomVector(j, b);
            
            for (int k = 0; k < s_dim; k++){
                if (i == 0){
                    mutantVector[j][k] = targetVector[j][k] + scalingFactor * (targetVector[b][k] - targetVector[c][k]);
                }
                mutantVector[j][k] = targetVector[j][k] + scalingFactor * (bestVector[k] - targetVector[j][k]) + scalingFactor * (targetVector[b][k] - targetVector[c][k]);
            }
        }

        // Crossover
        for (int j = 0; j < population_size; j++){
            double r = (double) rand() / (RAND_MAX + 1.0);
            q = 0 + (int) (r * (s_dim - 0));
            
            for (int k = 0; k < s_dim; k++){
                double rand_n = (double) rand() / (RAND_MAX + 1.0);
                if (rand_n <= controlParameter || j == q){
                    trialVector[j][k] = mutantVector[j][k];
                } else {
                    trialVector[j][k] = targetVector[j][k];
                }
            }
        }

        // Selection
        for (int j = 0; j < population_size; j++){
            double fitness_target = SphereEqu(targetVector[j]);
            double fitness_trial = SphereEqu(trialVector[j]);

            if (fitness_target > fitness_trial){ // trialVector is better (smaller)
                for (int k = 0; k < s_dim; k++){
                    targetVector[j][k] = trialVector[j][k];
                }
            }
            if (fitness_trial < min_fitness){
                for (int k = 0; k < s_dim; k++){
                    bestVector[k] = targetVector[j][k];
                }
                min_fitness = fitness_trial;
                min_iteration = i;
            }
        }
        if (i % 100){
            scalingFactor *= 0.99;
        }
    }
    cout << setw(4) << min_iteration << "        " << min_fitness << endl;
}

// Differential Evolution for Rastrigin equation
void DifferentialEvolution_R(int time){
    double targetVector[population_size][r_dim];
    double mutantVector[population_size][r_dim];
    double trialVector[population_size][r_dim];

    double scalingFactor = 1.2;

    int q; // guarantees at least one component from mutant vector
    double controlParameter = 0.8; // control diversity of the population

    double min_fitness = r_domain_max;
    int min_iteration = 0;

    double bestVector[r_dim]; // record the best target vector in the last iteration

    // Initialization: generate random seed
    for (int i = 0; i < population_size; i++){
        for (int j = 0; j < r_dim; j++){
            targetVector[i][j] = (r_domain_max - r_domain_min) * rand() / (RAND_MAX + 1.0) + r_domain_min;
        }
    }

    // iteration begin
    for (int i = 0; i < iteration; i++){

        // Mutation
        for (int j = 0; j < population_size; j++){
            // random select vector
            int b = randomVector(j, -1);
            int c = randomVector(j, b);
            
            for (int k = 0; k < r_dim; k++){
                if (i == 0){
                    mutantVector[j][k] = targetVector[j][k] + scalingFactor * (targetVector[b][k] - targetVector[c][k]);
                }
                mutantVector[j][k] = targetVector[j][k] + scalingFactor * (bestVector[k] - targetVector[j][k]) + scalingFactor * (targetVector[b][k] - targetVector[c][k]);
            }
        }

        // Crossover
        for (int j = 0; j < population_size; j++){
            double r = (double) rand() / (RAND_MAX + 1.0);
            q = 0 + (int) (r * (r_dim - 0));
            
            for (int k = 0; k < r_dim; k++){
                double rand_n = (double) rand() / (RAND_MAX + 1.0);
                if (rand_n <= controlParameter || j == q){
                    trialVector[j][k] = mutantVector[j][k];
                } else {
                    trialVector[j][k] = targetVector[j][k];
                }
            }
        }

        // Selection
        for (int j = 0; j < population_size; j++){
            double fitness_target = RastriginEqu(targetVector[j]);
            double fitness_trial = RastriginEqu(trialVector[j]);

            if (fitness_target > fitness_trial){ // trialVector is better (smaller)
                for (int k = 0; k < r_dim; k++){
                    targetVector[j][k] = trialVector[j][k];
                }
            }
            if (fitness_trial < min_fitness){
                for (int k = 0; k < r_dim; k++){
                    bestVector[k] = targetVector[j][k];
                }
                min_fitness = fitness_trial;
                min_iteration = i;
            }
        }
        if (i % 100){
            scalingFactor *= 0.99;
        }
    }
    cout << setw(4) << min_iteration << "        " << min_fitness << endl;
}

int main(void){

    srand(time(NULL));

    cout << "----- Sphere equation -----" << endl;
    cout << "iteration   min_fitness" << endl;
    for (int i = 0; i < exp_time; i++){
        DifferentialEvolution_S(i);
    }

    cout << "----- Rastrigin equation -----" << endl;
    cout << "iteration   min_fitness" << endl;
    for (int i = 0; i < exp_time; i++){
        //DifferentialEvolution_R(i);
    }

    return 0;
}