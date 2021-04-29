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
int population_size = 5;

// setting of Sphere equation
double s_domain_min = -100;
double s_domain_max = 100;
int s_dim = 4;

// setting of Rastrigin equation
double r_domain_min = -1.5;
double r_domain_max = 1.5;
int r_dim = 4;

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

// Particle Swarm Optimixation for Sphere equation
void PSO_S(int time){
    double velocity[population_size][s_dim];// Velocity of particle
    double particle[population_size][s_dim];// position of particle
    double pBest[s_dim]; // Personal best of particles
    double gBest[s_dim]; // Global best of particles

    double w = 0.7; // inertia
    double c1 = 1; // coefficient of pBest
    double c2 = 1.2; // coefficient of gBest
    double r1, r2;

    double personal_min_fitness = s_domain_max;
    double global_min_fitness = s_domain_max;
    int min_iteration = 0;

    double tmp_fitness = 0;

    // Initialization position and velocity randomly for each particle
    for (int i = 0; i < population_size; i++){
        for (int j = 0; j < s_dim; j++){
            double r = (double) rand() / (RAND_MAX + 1.0) + (-0.5);
            velocity[i][j] = ((s_domain_max - s_domain_min) * rand() / (RAND_MAX + 1.0) + s_domain_min) * r;
            particle[i][j] = (s_domain_max - s_domain_min) * rand() / (RAND_MAX + 1.0) + s_domain_min;
            pBest[j] = 0;
            gBest[j] = 0;
        }
    }

    // iteration begin
    for (int i = 0; i < iteration; i++){

        // Fitness and Update: find the pBest and gBest
        for (int j = 0; j < population_size; j++){
            tmp_fitness = SphereEqu(particle[j]);
            if (personal_min_fitness > tmp_fitness){
                personal_min_fitness = tmp_fitness;
                for (int k = 0; k < s_dim; k++){
                    pBest[k] = particle[j][k];
                }
                
                if (global_min_fitness > tmp_fitness){
                    global_min_fitness = tmp_fitness;
                    for (int k = 0; k < s_dim; k++){
                        gBest[k] = particle[j][k];
                    }
                    min_iteration = i;
                }
            }
        }

        // Move: Xt = Xt-1 + Vt
        for (int j = 0; j < population_size; j++){
            for (int k = 0; k < s_dim; k++){
                r1 = (double) rand() / (RAND_MAX + 1.0);
                r2 = (double) rand() / (RAND_MAX + 1.0);
                velocity[j][k] = w * velocity[j][k] + c1 * r1 * (pBest[k] - particle[j][k]) + c2 * r2 * (gBest[k] - particle[j][k]);
                particle[j][k] = particle[j][k] + velocity[j][k];

                // check the boundary
                if (particle[j][k] > s_domain_max){
                    particle[j][k] = s_domain_max;
                }
                if (particle[j][k] < s_domain_min){
                    particle[j][k] = s_domain_min;
                }
            }
        }
        
        if (i % 100){
            w -= 0.0001;
            c1 += 0.0001;
            c2 += 0.002;
        }
    }
    cout << setw(4) << min_iteration << "        " << global_min_fitness << endl;
}

// Particle Swarm Optimixation for Rastrigin equation
void PSO_R(int time){
    double velocity[population_size][r_dim];// Velocity of particle
    double particle[population_size][r_dim];// position of particle
    double pBest[r_dim]; // Personal best of particles
    double gBest[r_dim]; // Global best of particles

    double w = 0.7; // inertia
    double c1 = 1.5; // coefficient of pBest
    double c2 = 1.5; // coefficient of gBest
    double r1, r2;

    double personal_min_fitness = r_domain_max;
    double global_min_fitness = r_domain_max;
    int min_iteration = 0;

    double tmp_fitness = 0;

    // Initialization position and velocity randomly for each particle
    for (int i = 0; i < population_size; i++){
        for (int j = 0; j < s_dim; j++){
            double r = (double) rand() / (RAND_MAX + 1.0) + (-0.5);
            velocity[i][j] = ((r_domain_max - r_domain_min) * rand() / (RAND_MAX + 1.0) + r_domain_min) * r;
            particle[i][j] = (r_domain_max - r_domain_min) * rand() / (RAND_MAX + 1.0) + r_domain_min;
            pBest[j] = 0;
            gBest[j] = 0;
        }
    }

    // iteration begin
    for (int i = 0; i < iteration; i++){

        // Fitness and Update: find the pBest and gBest
        for (int j = 0; j < population_size; j++){
            tmp_fitness = RastriginEqu(particle[j]);
            if (personal_min_fitness > tmp_fitness){
                personal_min_fitness = tmp_fitness;
                for (int k = 0; k < r_dim; k++){
                    pBest[k] = particle[j][k];
                }
                
                if (global_min_fitness > tmp_fitness){
                    global_min_fitness = tmp_fitness;
                    for (int k = 0; k < r_dim; k++){
                        gBest[k] = particle[j][k];
                    }
                    min_iteration = i;
                }
            }
        }

        // Move: Xt = Xt-1 + Vt
        for (int j = 0; j < population_size; j++){
            for (int k = 0; k < r_dim; k++){
                r1 = (double) rand() / (RAND_MAX + 1.0);
                r2 = (double) rand() / (RAND_MAX + 1.0);
                velocity[j][k] = w * velocity[j][k] + c1 * r1 * (pBest[k] - particle[j][k]) + c2 * r2 * (gBest[k] - particle[j][k]);
                particle[j][k] = particle[j][k] + velocity[j][k];

                // check the boundary
                if (particle[j][k] > r_domain_max){
                    particle[j][k] = r_domain_max;
                }
                if (particle[j][k] < r_domain_min){
                    particle[j][k] = r_domain_min;
                }
            }
        }
    }
    cout << setw(4) << min_iteration << "        " << global_min_fitness << endl;
}

int main(void){

    srand(time(NULL));

    cout << "----- Sphere equation -----" << endl;
    cout << "iteration   min_fitness" << endl;
    for (int i = 0; i < exp_time; i++){
        PSO_S(i);
    }

    cout << "----- Rastrigin equation -----" << endl;
    cout << "iteration   min_fitness" << endl;
    for (int i = 0; i < exp_time; i++){
        PSO_R(i);
    }

    return 0;
}