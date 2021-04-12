#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath> 
using namespace std;

double PI = 3.141592653589793116;

// number of iterations
int iteration = 1000;

// domain of Sphere equation
double s_domain_min = -100;
double s_domain_max = 100;

// domain of Rastrigin equation
double r_domain_min = -1.5;
double r_domain_max = 1.5;

// input of Sphere equation
double SphereEqu(double x){
    // check if x is out of domain
    if (x < s_domain_min){
        x = s_domain_min;
    }
    if (x > s_domain_max){
        x = s_domain_max;
    }

    return pow(x, 2);
}

// input of Rastrigin equation
double RastriginEqu(double x){
    // check if x is out of domain
    if (x < r_domain_min){
        x = r_domain_min;
    }
    if (x > r_domain_max){
        x = r_domain_max;
    }

    double y = 10 + (pow(x, 2) - 10 * cos(2 * PI * x));
    return y;
}

// Simulated Annealing algorithm for Sphere equation
void simulatedAnnealing_S(double current_posX){
    double pace = (s_domain_max - s_domain_min) / 1000;
    double temp = (s_domain_max - s_domain_min) / 100;

    int count_worse = 0; // count the stuck iteration
    
    double p; // the probability of accept the neighbor f(w')
    double p_rand;  

    double current_posY = SphereEqu(current_posX);

    double min_iteration = 0; // the iteration that find the answer 
    double min_ans = current_posY; // answer

    double last_posX, last_posY;

    double l_nei, r_nei; // left neighbor & right neighbor
    double nei_rand;

    for (int i = 1; i <= iteration; i++){
        l_nei = SphereEqu(current_posX - pace);
        r_nei = SphereEqu(current_posX + pace);

        last_posX = current_posX;
        last_posY = current_posY;
        
        // find a random neighbor
        nei_rand = (double) rand() / (RAND_MAX + 1.0);

        if (nei_rand < 0.5){
            current_posY = l_nei;
            current_posX -= pace;
        } else {
            current_posY = r_nei;
            current_posX += pace;
        }

        // decide accept the neighbor or not
        if (current_posY - last_posY <= 0){ // current better than last
            p = 1;
        } else {
            p = exp(-(current_posY - last_posY) / temp);
        }

        p_rand = (double) rand() / (RAND_MAX + 1.0);

        if(p < p_rand){ // don't accept the neighbor
            current_posX = last_posX;
            current_posY = last_posY;
        }

        // find the best answer
        if (current_posY < min_ans){
            min_iteration = i;
            min_ans = current_posY;
            count_worse = 0;
        } else {
            count_worse++;
        }
        
        // adjust the pace size
        if (i % 15 == 0){
            double slope = (current_posY - last_posY) / (current_posX - last_posX);

            if (abs(slope) > 3){ // steeper, around 75 degree
                pace = pace * 2;
            } else if (abs(slope) > 1){ // 45 degree
                pace += pace / 2;
            } else if (abs(slope) > 0.5){ // smoother, 25 degree
                pace -= pace / 2;
            } else { 
                pace = pace / 1.2;
            }
        }

        // adjust the temp - cooling
        if (i < 200 && count_worse > 20) {
            temp *= 1.5;
            count_worse = 0;
        }
        
        if (i >= 200 && i % 5 == 0){ 
            temp *= 0.96;
        }
    }
    cout << "Simulated Annealing algorithm for Sphere equation:" << endl;
    cout << min_iteration << " th, ans= " << min_ans << endl;
}

// Simulated Annealing algorithm for Rastrigin equation
void simulatedAnnealing_R(double current_posX){
    double pace = (r_domain_max - r_domain_min) / 1000;
    double temp = (s_domain_max - s_domain_min) / 100;

    int count_worse = 0; // count the stuck iteration
    
    double p; // the probability of accept the neighbor f(w')
    double p_rand;
    
    double current_posY = RastriginEqu(current_posX);

    double min_iteration = 0; // the iteration that find the answer 
    double min_ans = current_posY; // answer

    double last_posX, last_posY;

    double l_nei, r_nei; // left neighbor & right neighbor
    double nei_rand;

    for (int i = 1; i <= iteration; i++){
        l_nei = RastriginEqu(current_posX - pace);
        r_nei = RastriginEqu(current_posX + pace);

        last_posX = current_posX;
        last_posY = current_posY;
        
        // find a random neighbor
        nei_rand = (double) rand() / (RAND_MAX + 1.0);

        if (nei_rand < 0.5){
            current_posY = l_nei;
            current_posX -= pace;
        } else {
            current_posY = r_nei;
            current_posX += pace;
        }

        // decide accept the neighbor or not
        if (current_posY - last_posY <= 0){ // current better than last
            p = 1;
        } else {
            p = exp(-(current_posY - last_posY) / temp);
        }

        p_rand = (double) rand() / (RAND_MAX + 1.0);

        if (p < p_rand){ // don't accept the neighbor
            current_posX = last_posX;
            current_posY = last_posY;
        }

        // find the best answer
        if (current_posY < min_ans){
            min_iteration = i;
            min_ans = current_posY;
            count_worse = 0;
        } else {
            count_worse++;
        }
        
        // adjust the pace size
        if ((i < 500 && i % 20 == 0) || (i >= 500 && i % 15 == 0)){
            double slope = (current_posY - last_posY) / (current_posX - last_posX);

            if (abs(slope) > 3){ // steeper, around 75 degree
                pace = pace * 2;
            } else if (abs(slope) > 1){ // 45 degree
                pace += pace / 2;
            } else if (abs(slope) > 0.5){ // smoother, 25 degree
                pace -= pace / 2;
            } else { 
                pace = pace / 2;
            }
        }

        // adjust the temp - cooling
        if (i < 150 && count_worse > 20) {
            temp *= 2;
            count_worse = 0;
        }

        if (i > 150 && i % 5 == 0) {
            temp *= 0.99;
        }

        if (i > 500) {
            temp *= 0.98;
        }

        if (i > 700) {
            temp *= 0.97;
        }
    }

    cout << "Simulated Annealing algorithm for Rastrigin equation:" << endl;
    cout << min_iteration << " th, ans= " << min_ans << endl;
}

int main(void){
    // the first position
    double s_begin = -99.7497;
    double r_begin = -1.49625;

    double random_num = 5;

    cout << "--------- 第一次撒點 ---------" << endl; 
    cout << "s 位置:" << s_begin << ", r 位置:" << r_begin << endl;

    simulatedAnnealing_S(s_begin);
    simulatedAnnealing_R(r_begin);

    double s_rand, r_rand;

    srand(time(NULL));

    for(int i = 1; i <= random_num; i++){
        s_rand = (s_domain_max - s_domain_min) * rand() / (RAND_MAX + 1.0) + s_domain_min;
        r_rand = (r_domain_max - r_domain_min) * rand() / (RAND_MAX + 1.0) + r_domain_min;

        cout << "--------- 第 " << i << " 次隨機撒點 ---------" << endl; 
        cout << "s 位置:" << s_rand << ", r 位置:" << r_rand << endl;

        simulatedAnnealing_S(s_rand);
        simulatedAnnealing_R(r_rand);
    }
    return 0;
}