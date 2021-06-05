#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip> 
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

double PI = 3.141592653589793116;

// setting of equation
class Equation{
    public:
        double domain_min;
        double domain_max;
        int dim;
        Equation(double min, double max, int d){
            domain_min = min;
            domain_max = max;
            dim = d;
        }
        virtual double formula (double x[]) { return 100;}
};

class SphereEqu: public Equation{
    public:
        SphereEqu(double min, double max, int d): Equation(min, max, d){}
        double formula(double x[]){
            double result = 0;

            for (int i = 0; i < dim; i++){
                result += pow(x[i], 2);
            }
            return result;
        }
};

class RastriginEqu: public Equation{
    public:
        RastriginEqu(double min, double max, int d): Equation(min, max, d){}
        double formula(double x[]){
            double result = 10 * dim;

            for (int i = 0; i < dim; i++){
                result += (pow(x[i], 2) - 10 * cos(2 * PI * x[i]));
            }
            return result;
        }
};

class SchwefelEqu: public Equation{
    public:
        SchwefelEqu(double min, double max, int d): Equation(min, max, d){}
        double formula(double x[]){
            double result = 418.9829 * dim;
            double sum = 0;

            for (int i = 0; i < dim; i++){
                sum += x[i] * sin(sqrt(abs(x[i])));
            }
            result -= sum;

            return result;
        }
};

// calculate the fitness of particle, it's the opposite force of f(x)
double fitnessFuction(double f){
    double y;

    if (f >= 0){
        y = 1 / (1 + f);
    } else {
        y = 1 + abs(f);
    }
    return y;
}

// randomly selected partner from population list
int getParter(int swarm_size, int a){
    double r = (double) rand() / RAND_MAX;
    int c = 0 + (int) (r * (swarm_size - 0));

    while (c == a){
        r = (double) rand() / RAND_MAX;
        c = 0 + (int) (r * (swarm_size - 0));
    }
    return c;
}

// Check if x is out of domain
double check_domain(double domain_min, double domain_max, double x){
    if (x < domain_min){
        x = domain_min;
    }
    if (x > domain_max){
        x = domain_max;
    }
    return x;
}

// select the employed bee according to probability
int selectEmployedBee(int swarm_size, double prob[]){
    double r = (double) rand() / RAND_MAX;
    int i;
cout << "r = " << r;
    for (i = 0; i < swarm_size; i++){
        if (r <= prob[i]){
            break;
        }
    }
    return i;
}

void move(Equation * equ, int swarm_size, double particlePos[], double partnerPos[], double candidatePos[], double bestPos[]){
    // Random select some dims to change
    double r = (double) rand() / RAND_MAX;
    int k = 0 + (int) (r * (equ->dim - 0));

    // Random scaling factor of moving distance
    double f = (double) rand() / RAND_MAX / 2; // [0,0.5]
    r = (double) (1-(-1)) * rand() / RAND_MAX + (-1); // [-1,1]

    for (int j = 0; j < equ->dim; j++){
        if (j == k){
            candidatePos[j] = particlePos[j] + f * (bestPos[j] - particlePos[j]) + r * (particlePos[j] - partnerPos[j]);
            candidatePos[j] = check_domain(equ->domain_min, equ->domain_max, candidatePos[j]);
        } else {
            candidatePos[j] = particlePos[j];
        }
    }
}

// select the position which has the better fitness
void greedySelection(Equation * equ, int index, double candidatePos[], double particleFitness[], double particlePos[], double trialCounter[], int state){
    double candidateFitness = equ->formula(candidatePos);

    // state = 0, search for maximum; state = 1, search for minimum
    if ((state == 0 && candidateFitness >= particleFitness[index]) || (state == 1 && candidateFitness <= particleFitness[index])){
        // update the current solution
        for (int k = 0; k < equ->dim; k++){
            particlePos[k] = candidatePos[k];
        }
        particleFitness[index] = candidateFitness;
        trialCounter[index] = 0;  
    } else {
        trialCounter[index]++;
    }
}

// Artificial Bee Colony 
void ABC(Equation * equ, int swarm_size, int iteration, double limit, int state, double equRecord[]){
    double particlePos[swarm_size][equ->dim];// position of particle
    double particle_fx[swarm_size]; // f(x) of particle
    double particle_fitness[swarm_size]; // fitness of particle, it's the opposite force of f(x)
    double candidatePos[equ->dim];// candidate position of particle

    double trialCounter[swarm_size]; // track the number of failures encountered by each solution
    double prob[swarm_size]; // the probability of food source is chosen by an onlooker

    double best_fitness;
    int best_iteration;
    double bestSolution[equ->dim]; // position of best solution ever

    double r = 0;

    // Initialization: random position for each particle, and intialize other arrays
    for (int i = 0; i < swarm_size; i++){
        trialCounter[i] = 0;
        prob[i] = 0;

        for (int j = 0; j < equ->dim; j++){
            r = (double) rand() / RAND_MAX;
            particlePos[i][j] = (equ->domain_max - equ->domain_min) * r + equ->domain_min;

            // Give best solution an random initial value (particlePos[0])
            if (i == 0){
                bestSolution[j] = particlePos[i][j];
            }
        }
        particle_fx[i] = equ->formula(particlePos[i]);
    }

    int partner; // index of the partner of bee in population

    // iteration begin
    for (int i = 0; i < iteration; i++){
        /* ---- Employed Bee ---- */
        /* produce a candidate food position from the old one, and greedy select the better position */
        for (int j = 0; j < swarm_size; j++){
            // Select the random partner
            partner = getParter(swarm_size, j);

            move(equ, swarm_size, particlePos[j], particlePos[partner], candidatePos, bestSolution);

            greedySelection(equ, j, candidatePos, particle_fx, particlePos[j], trialCounter, state);
        }
        /* ---- End of Employed Bee ---- */

        /* ---- Onlooker Bee ---- */
        /* produce the probability of selecting food source, and produce a candidate food position
           finally, greedy select the better position */
        double sumFitness = 0;

        for (int j = 0; j < swarm_size; j++){
            if (state == 0){
                sumFitness += particle_fx[j];
            } else {
                particle_fitness[j] = fitnessFuction(particle_fx[j]);
                sumFitness += particle_fitness[j];
            }
        }
    
        for (int j = 0; j < swarm_size; j++){
            if (state == 0){
                prob[j] = particle_fx[j] / sumFitness;
            } else {
                prob[j] = particle_fitness[j] / sumFitness;
            }
        }

        // From the perspective of onlooker bee, looking at one food source at a time
        int index = 0;
        for (int j = 0; j < swarm_size; j++){
            r = (double) rand() / RAND_MAX;
            while (r > prob[index]){
                r = (double) rand() / RAND_MAX;
                index++;
                index %= swarm_size;
            }

            partner = getParter(swarm_size, index);

            move(equ, swarm_size, particlePos[index], particlePos[partner], candidatePos, bestSolution);

            greedySelection(equ, index, candidatePos, particle_fx, particlePos[index], trialCounter, state);
            index++;
            index %= swarm_size;
        }
        /* ---- End of Onlooker Bee ---- */

        /* ---- Scout Bee ---- */
        /* The food source of which is abandoned by the bees is replaced with a new food source by the scouts */
        int count = 0; // number of food source which trailCounter > limit
        vector<int> recordList;
        for (int j = 0; j < swarm_size; j++){
            if (trialCounter[j] > limit){
                count++;
                recordList.push_back(j);
            }
        }

        if (count > 0){
            // produce a random position
            double randomPos[swarm_size]; // the random position of scout bee
            for (int k = 0; k < swarm_size; k++){
                double r = (double) rand() / RAND_MAX;
                randomPos[k] = (equ->domain_max - equ->domain_min) * r + equ->domain_min;
            }

            int maxIndex = recordList[0];

            if (count > 1){
                // find the max trailCounter
                maxIndex = distance(trialCounter, max_element(trialCounter, trialCounter+swarm_size));
            }

            // update the position
            trialCounter[maxIndex] = 0;
            for (int k = 0; k < equ->dim; k++){
                particlePos[maxIndex][k] = randomPos[k];
                particle_fx[maxIndex] = equ->formula(particlePos[maxIndex]);
                particle_fitness[maxIndex] = fitnessFuction(particle_fx[maxIndex]);
            }
        }
        /* ---- End of Scout Bee ---- */
        
        /* ---- Memorize ----*/ 
        /* record the information of best solution */
        double result;

        if (state == 0){
            result = *max_element(particle_fx, particle_fx+swarm_size);
            index = distance(particle_fx, max_element(particle_fx, particle_fx+swarm_size));
        } else {
            result = *min_element(particle_fx, particle_fx+swarm_size);
            index = distance(particle_fx, min_element(particle_fx, particle_fx+swarm_size));
        }
        
        if (i == 0){
            best_fitness = result;
            best_iteration = i;
            for (int j = 0; j < equ->dim; j++){
                bestSolution[j] = particlePos[index][j];
            }
        } else {
            if ((state == 0 && result > best_fitness) || (state == 1 && result < best_fitness)){ // have the solution better than bestSolution
                best_fitness = result;
                best_iteration = i;
                for (int j = 0; j < equ->dim; j++){
                    bestSolution[j] = particlePos[index][j];
                }
            }
        }
        if (i > 300 && i % 150 == 0 && limit > 0){
            limit--;
        }
    }
    equRecord[0] += best_iteration;
    equRecord[1] += best_fitness;
    //cout << setw(4) << best_iteration << "        " << best_fitness << ", limit = " << limit << endl;
}

int main(void){

    srand(time(NULL));

    // experience environment
    int iteration = 1000;
    int exp_time = 50;
    int sn = 10; // SN = num of employed bees = num of onlooker bees
    int dim[2] = {2, 30}; 
    int state = 1; // state = 0, searching for maximum solution; state = 1, searching for minimum solution

    double limit; // threshold of failure times

    int dimLength = sizeof(dim)/sizeof(dim[0]);

    cout << "----- Sphere equation -----" << endl;
    cout << "dim" << setw(12) << "iteration" << setw(15) << "best_fitness" << endl;
    for (int i = 0; i < dimLength; i++){
        SphereEqu sphereEqu(-100, 100, dim[i]);
        Equation * sphere = &sphereEqu;
        double sphereRecord[2] = {0};

        if (dim[i] > 20){
            limit = 50;
        } else {
            limit = 30;
        }
        
        for (int j = 0; j < exp_time; j++){    
            ABC(sphere, sn, iteration, limit, state, sphereRecord);
        }
        cout << setw(3) <<  dim[i] << setw(12) << sphereRecord[0] / exp_time << setw(15) << sphereRecord[1] / exp_time << endl;
    }

    cout << "----- Rastrigin equation -----" << endl;
    cout << "dim" << setw(12) << "iteration" << setw(15) << "best_fitness" << endl;
    for (int i = 0; i < dimLength; i++){
        RastriginEqu rastriginEqu(-1.5, 1.5, dim[i]);
        Equation * rastrigin = &rastriginEqu;
        double rastriginRecord[2] = {0};

        if (dim[i] > 20){
            limit = 50;
        } else {
            limit = 30;
        }

        for (int j = 0; j < exp_time; j++){    
            ABC(rastrigin, sn, iteration, limit, state, rastriginRecord);
        }
        cout << setw(3) <<  dim[i] << setw(12) << rastriginRecord[0] / exp_time << setw(15) << rastriginRecord[1] / exp_time << endl;
    }

    cout << "----- Schwefel equation -----" << endl;
    cout << "dim" << setw(12) << "iteration" << setw(15) << "best_fitness" << endl;
    for (int i = 0; i < dimLength; i++){
        SchwefelEqu schwefelEqu(-500, 500, dim[i]);
        Equation * schwefel = &schwefelEqu;
        double schwefelRecord[2] = {0};

        if (dim[i] > 20){
            limit = 30;
        } else {
            limit = 10;
        }

        for (int j = 0; j < exp_time; j++){ 
            ABC(schwefel, sn, iteration, limit, state, schwefelRecord);
        }
        cout << setw(3) <<  dim[i] << setw(12) << schwefelRecord[0] / exp_time << setw(15) << schwefelRecord[1] / exp_time << endl;
    }
    return 0;
}