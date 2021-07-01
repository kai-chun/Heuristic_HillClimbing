#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip> 
#include <algorithm>
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
using namespace std;

double PI = 3.141592653589793116;

// setting of equation
class Equation{
    public:
        double domain_min;
        double domain_max;
        int dim;
        string name; 
        string type; // unimodal, multimodal
        Equation(double min, double max, int d){
            domain_min = min;
            domain_max = max;
            dim = d;
        }
        virtual double formula (vector<double> x) { return 100;}
};

class SphereEqu: public Equation{
    public:
        SphereEqu(double min, double max, int d): Equation(min, max, d){
            name = "Sphere";
            type = "unimodal";
        }
        double formula(vector<double> x){
            double result = 0;

            for (int i = 0; i < dim; i++){
                result += pow(x[i], 2);
            }
            return result;
        }
};

class RastriginEqu: public Equation{
    public:
        RastriginEqu(double min, double max, int d): Equation(min, max, d){
            name = "Rastrigin";
            type = "multimodal";
        }
        double formula(vector<double> x){
            double result = 10 * dim;

            for (int i = 0; i < dim; i++){
                result += (pow(x[i], 2) - 10 * cos(2 * PI * x[i]));
            }
            return result;
        }
};

class SchwefelEqu: public Equation{
    public:
        SchwefelEqu(double min, double max, int d): Equation(min, max, d){
            name = "Schwefel";
            type = "multimodal";
        }
        double formula(vector<double> x){
            double result = 418.9829 * dim;
            double sum = 0;

            for (int i = 0; i < dim; i++){
                sum += x[i] * sin(sqrt(abs(x[i])));
            }
            result -= sum;

            return result;
        }
};

class SimpletonEqu: public Equation{
    public:
        SimpletonEqu(double min, double max, int d): Equation(min, max, d){
            name = "Simpleton";
            type = "unimodal";
        }
        double formula(vector<double> x){
            double result = 0;

            for (int i = 0; i < dim; i++){
                result += x[i];
            }
            return result;
        }
};

class RosenbrockEqu: public Equation{
    public:
        RosenbrockEqu(double min, double max, int d): Equation(min, max, d){
            name = "Rosenbrock";
            type = "unimodal";
        }
        double formula(vector<double> x){
            double result = 0;
            for (int i = 0; i < dim-1; i++){
                result += 100 * pow(x[i+1] - pow(x[i], 2),2) + pow(x[i] - 1 ,2);
            }
            return result;
        }
};

class Frog{
    public:
        vector<double> pos;
        double fitness;
        double prob;
        void setPos(vector<double> p){
            pos = p;
        }
        void setFitness(double f, int * count){
            fitness = f;
            *count += 1;
        }
        // Pj = 2 * (n + 1 - j) / n * (n + 1), j = 1, ..., n  
        void setProb(int j, int n){
            prob = (double) (2 * (n + 1 - j)) / (n * (n + 1));
        }
        void print(int dim){
            cout << "pos = ";
            for (int j = 0; j < dim; j++){
                cout << pos[j] << ", ";
            }
            cout <<  "f = " << fitness;
            cout <<  ", p = " << prob << endl;
        }
};

// initialize the position
vector<double> initial(Equation * equ){
    double r;
    vector<double> pos(equ->dim);

    for (int j = 0; j < equ->dim; j++){
        r = (double) rand() / RAND_MAX;
        pos[j] = (equ->domain_max - equ->domain_min) * r + equ->domain_min;
    }
    return pos;
}

class SFLA {
    public:
        SFLA(int m, int n, int q, int N, double Smax);
        int get_m(){ return m; }
        int get_n(){ return n; }
        int get_q(){ return q; }
        int get_N(){ return N; }
        double get_Smax(){ return Smax; }
        void set_count(int n) { count = n; }

        int count; // count the computed number

    private:
        int m; // number of memeplexes
        int n; // number of frogs in each memeplex
        int q; // number of frogs in each submemeplex
        int N; // the maximum evolutionary step = N
        double Smax; // the maximum step size
};

SFLA::SFLA(int a, int b, int c, int d, double e){
    m = a;
    n = b;
    q = c;
    N = d;
    Smax = e;
    count = 0;
}

/* comparator for Frog sorting by fitness 
   state = 0, large -> small; state = 1, small -> large */
bool sortFrog0(const Frog a, const Frog b){
    return (a.fitness > b.fitness);
}
bool sortFrog1(const Frog a, const Frog b){
    return (a.fitness < b.fitness);
}

// print position, fitness, probability of frog 
void printFrog(vector<Frog> frogs, int dim){
    for (int i = 0; i < frogs.size(); i++){
        cout << i << ", pos = ";
        for (int j = 0; j < dim; j++){
            cout << frogs[i].pos[j] << ", ";
        }
        cout <<  "f = " << frogs[i].fitness;
        cout <<  ", p = " << frogs[i].prob << endl;
    }
    cout << endl;
}

/* select the frog of submemeplex by triangular probability, 
   and select by searching a random number that falls in which prob[] interval */
vector<vector<Frog> > selectFrog0(vector<Frog> memeplex, int q){
    vector<Frog> tmp, submemeplex;
    vector<vector<Frog> > result;
    vector<int> exist;
    double prob[memeplex.size()];
    
    double sumProb = 0;
    for (int i = 0; i < memeplex.size(); i++){
        sumProb += memeplex[i].prob;
        prob[i] = sumProb;
    }

    int index;
    
    while (exist.size() < q){
        double r = (double) rand() / RAND_MAX;

        for (index = 0; index < memeplex.size(); index++){
            if (r <= prob[index]){
                break;
            }
        }

        if (find(exist.begin(), exist.end(), index) == exist.end()){
            exist.push_back(index);
        }
    }

    for (int i = 0; i < q; i++){
        submemeplex.push_back(memeplex[exist[i]]);
    }

    for (int i = 0; i < memeplex.size(); i++){
        if (find(exist.begin(), exist.end(), i) == exist.end()){
            tmp.push_back(memeplex[i]);
        }
    }
    result.push_back(submemeplex);
    result.push_back(tmp);

    return result;
}

/* select the frog of submemeplex by triangular probability, 
   and select by random a number compare to each frog's probability */
vector<vector<Frog> > selectFrog1(vector<Frog> memeplex, int q){
    vector<Frog> tmp, submemeplex;
    vector<vector<Frog> > result;
    vector<int> exist;

    int index = 0;

    while (exist.size() < q){
        double r = (double) rand() / RAND_MAX;

        if (r < memeplex[index].prob){
            if (find(exist.begin(), exist.end(), index) == exist.end()){
                exist.push_back(index);
            }
        }
        index = (index + 1) % memeplex.size();
    }

    for (int i = 0; i < q; i++){
        submemeplex.push_back(memeplex[exist[i]]);
    }

    for (int i = 0; i < memeplex.size(); i++){
        if (find(exist.begin(), exist.end(), i) == exist.end()){
            tmp.push_back(memeplex[i]);
        }
    }
    result.push_back(submemeplex);
    result.push_back(tmp);

    return result;
}

// Check if x is out of domain
int check_domain(Equation * equ, vector<double> x){
    for (int i = 0; i < x.size(); i++){
        if (x[i] < equ->domain_min || x[i] > equ->domain_max){
            return 1;
        }
    }
    return 0;
}

// compute step and new position for the worst frog, all dimension
Frog move0(Equation * equ, Frog worst, Frog best, double Smax, SFLA * sfla){
    Frog newFrog = worst;
    double step, r;

    for (int i = 0; i < equ->dim; i++){
        step = best.pos[i] - worst.pos[i];
        r = (double) rand() / RAND_MAX;
        // cout << "dim = " << i << ", r = " << r << endl;

        if (step >= 0){ // choose min
            if (step * r < Smax){
                step = step * r;
            } else {
                step = Smax;
            }
        } else { // choose max
            if (step * r > (-Smax)){
                step = step * r;
            } else {
                step = (-Smax);
            } 
        }
        newFrog.pos[i] += step;
    }
    newFrog.setFitness(equ->formula(newFrog.pos), &sfla->count);
    return newFrog;
}

// compute step and new position for the worst frog, one random dimension
Frog move1(Equation * equ, Frog worst, Frog best, double Smax, SFLA * sfla){
    Frog newFrog = worst; 
    int c = rand() % equ->dim; // Random select a dimension
    
    double step = best.pos[c] - worst.pos[c];
    double r = (double) rand() / RAND_MAX;
    //cout << "dim = " << c << ", r = " << r << endl;

    if (step >= 0){ // choose min
        if (step * r < Smax){
            step = step * r;
        } else {
            step = Smax;
        }
    } else { // choose max
        if (step * r > (-Smax)){
            step = step * r;
        } else {
            step = (-Smax);
        }
    }
    newFrog.pos[c] += step;
    newFrog.setFitness(equ->formula(newFrog.pos), &sfla->count);

    return newFrog;
}

// select the frog which has the better fitness
int greedy(Frog worst, Frog newFrog, int state){
    if ((state == 0 && sortFrog0(worst, newFrog)) || (state == 1 && sortFrog1(worst, newFrog))){
        return 1;
    }
    return 0;
}

// // the local exploration of SFLA : Frog-Leaping Algorithm
vector<Frog> FrogLeapingAlgo(Equation * equ, vector<Frog> memeplex, Frog gBest, SFLA * sfla, double Smax, int state){
    vector<Frog> submemeplex, tmp, memeplexNew;
    vector<vector<Frog> > result;
    Frog pBest, worst, newFrog;
    int evolStep = sfla->get_N();
    int q = sfla->get_q();

    for (int i = 0; i < memeplex.size(); i++){
        memeplexNew.push_back(memeplex[i]);
    }

    for (int iN = 0; iN < evolStep; iN++){
        /* --- Construct a submemeplex --- */
        //cout << "    --- Submemeplex " << iN <<  " --- " << endl;
        result = selectFrog0(memeplexNew, q);
        submemeplex = result[0];
        tmp = result[1];
        if (state == 0){
            sort(submemeplex.begin(), submemeplex.end(), &sortFrog0);
        } else {
            sort(submemeplex.begin(), submemeplex.end(), &sortFrog1);
        }
        //printFrog(memeplex, equ->dim);

        if (state == 0){
            sort(submemeplex.begin(), submemeplex.end(), &sortFrog0);
        } else {
            sort(submemeplex.begin(), submemeplex.end(), &sortFrog1);
        }
        pBest = submemeplex.front();
        worst = submemeplex.back();

        /* --- Improve the worst frog’s position --- */
        //cout << "    --- Improve --- " << endl;
        if (equ->type == "unimodal"){
            newFrog = move0(equ, worst, pBest, Smax, sfla);
        } else {
            newFrog = move0(equ, worst, pBest, Smax, sfla);
        }
        //worst.print(equ->dim);
        //newFrog.print(equ->dim);
        
        int trialCounter = check_domain(equ, newFrog.pos) + greedy(worst, newFrog, state);

        if (trialCounter != 0){ // move according gBest's position
            //cout << "    [ step 5 ]" << endl;
            if (equ->type == "unimodal"){
                newFrog = move0(equ, worst, gBest, Smax, sfla);
            } else {
                newFrog = move0(equ, worst, gBest, Smax, sfla);
            }
            trialCounter = check_domain(equ, newFrog.pos) + greedy(worst, newFrog, state);
            //sfla->count -= 1;
            //worst.print(equ->dim);
            //newFrog.print(equ->dim);

            if (trialCounter != 0){ // Censorship
                //cout << "    [ step 6 ]" << endl;
                vector<double> pos = initial(equ);
                newFrog.setPos(pos);
                newFrog.setFitness(equ->formula(pos), &sfla->count);
                //sfla->count -= 1;
                
                //newFrog.print(equ->dim);
            }
        }
        
        /* --- Upgrade the memeplex --- */
        // cout << "    [ step 7 ]" << endl;
        submemeplex.back() = newFrog;
        
        memeplexNew.clear();
        for (int j = 0; j < q; j++){
            memeplexNew.push_back(submemeplex[j]);
        }
        for (int j = 0; j < tmp.size(); j++){
            memeplexNew.push_back(tmp[j]);
        }

        if (state == 0){
            sort(memeplexNew.begin(), memeplexNew.end(), &sortFrog0);
        } else {
            sort(memeplexNew.begin(), memeplexNew.end(), &sortFrog1);
        }

        for (int j = 0; j < memeplexNew.size(); j++){
            memeplexNew[j].setProb(j+1, memeplexNew.size());
        }
        //printFrog(memeplexNew, equ->dim);
    }
    return memeplexNew;
}

// Shuffled frog-leaping algorithm
vector<double> ShuffledFrogLeapingAlgo (Equation * equ, SFLA * sfla, int computeNum, int state){
    vector<double> record(2);
    int memeplexNum = sfla->get_m();
    int memeplexFrogNum = sfla->get_n();
    int submemeplexFrogNum = sfla->get_q();
    int evolStep = sfla->get_N(); 
    sfla->set_count(0);

    /* --- Initialize --- */
    int frogNum = memeplexNum * memeplexFrogNum; // F = m * n
    vector<Frog> frogs(frogNum);
    double Smax = (equ->domain_max - equ->domain_min) * sfla->get_Smax();

    /* --- Generate a virtual population --- */
    //cout << "===== Initial =====" << endl;
    double r = 0;
    for (int i = 0; i < frogNum; i++){
        vector<double> pos = initial(equ);
        frogs[i].setPos(pos);
        frogs[i].setFitness(equ->formula(pos), &sfla->count);
    }
    //printFrog(frogs, equ->dim);

    // --- Rank frogs --- 
    //cout << "===== Rank =====" << endl;
    if (state == 0){
        sort(frogs.begin(), frogs.end(), &sortFrog0);
    } else {
        sort(frogs.begin(), frogs.end(), &sortFrog1);
    }
    //printFrog(frogs, equ->dim);

    // setting the gBest frog in first iteration
    Frog gBest = frogs.front();
    record[0] = 0;
    record[1] = gBest.fitness;

    while (sfla->count < computeNum){
        // --- Partition frogs into memeplexes --- 
        //cout << "===== Partition =====" << endl << endl;
        vector<Frog> memeplex[memeplexNum];
        for (int j = 0; j < frogNum; j++){
            memeplex[j%memeplexNum].push_back(frogs[j]);
        }
        for (int j = 0; j < memeplexNum; j++){
            //cout << "[ Group " << j << " ]" << endl;
            for (int k = 0; k < memeplex[j].size(); k++){
                memeplex[j][k].setProb(k+1, memeplex[j].size());
            }
            //printFrog(memeplex[j], equ->dim);
        }
        frogs.clear();

        // --- Memetic evolution --- 
        //cout << "===== Memetic evolution =====" << endl << endl;
        vector<Frog> memeplexNew[memeplexNum];
        for (int im = 0; im < memeplexNum; im++){
            memeplexNew[im] = FrogLeapingAlgo(equ, memeplex[im], gBest, sfla, Smax, state);
            //printFrog(memeplexNew[im], equ->dim);
            
            for (int j = 0; j < memeplexNew[im].size(); j++){
                frogs.push_back(memeplexNew[im][j]);
            }
        }
        //printFrog(frogs, equ->dim);

        // --- Update --- 
        //cout << "===== Update =====" << endl << endl;
        if (state == 0){
            sort(frogs.begin(), frogs.end(), &sortFrog0);
        } else {
            sort(frogs.begin(), frogs.end(), &sortFrog1);
        }
        if (sfla->count == 0){
            //printFrog(frogs, equ->dim);
        }
        
        gBest = frogs.front();

        if ((state == 0 && gBest.fitness > record[1]) || (state == 1 && gBest.fitness < record[1])){
            record[0] = sfla->count;
            record[1] = gBest.fitness;
        }
    }
    return record;
}

void run(Equation * equ, SFLA * algo, int exp_time, int computeNum, int state, double list[2][100]){
    cout << "----- " << equ->name << " equation -----" << endl;
    cout << "dim" << setw(12) << "computeNum" << setw(15) << "best_fitness" << endl;

    vector<double> record(2);
    vector<double> sum(2, 0);

    for (int j = 0; j < exp_time; j++){    
        record = ShuffledFrogLeapingAlgo(equ, algo, computeNum, state);
        sum[0] += record[0];
        sum[1] += record[1];
        list[0][j] = record[0];
        list[1][j] = record[1];
    }
    double avg[2];
    avg[0] = sum[0] / exp_time;
    avg[1] = sum[1] / exp_time;

    cout << setw(3) <<  equ->dim << setw(12) << avg[0] << setw(15) << avg[1] << endl;

    double SD[2] = {0}; // Standard Deviation
    
    for (int i = 0; i < exp_time; i++){
        SD[0] += pow(list[0][i]-avg[0] ,2);
        SD[1] += pow(list[1][i]-avg[1] ,2);
    }

    SD[0] = sqrt(SD[0] / exp_time);
    SD[1] = sqrt(SD[1] / exp_time);

    //cout << setw(3) << "SD" << setw(12) << SD[0] << setw(15) << SD[1] << endl;
}

int main(void){

    srand(time(NULL));

    // experience environment
    int computeNum = 100000;
    int exp_time = 100;

    int m = 5; // number of memeplexes
    int n = 5; // number of frogs in each memeplex
    int evolStep = 5; // the maximum evolutionary step = N
    int q = 3; // number of frogs in each submemeplex
    double Smax_scale = 0.6;

    int dim = 30; 
    int state = 1; // state = 0, searching for maximum solution; state = 1, searching for minimum solution
/*
    // print
    ofstream excelFile0, excelFile1;
    excelFile0.open("SA_" + to_string(dim) + "_compute.csv");
    excelFile1.open("SA_" + to_string(dim) + "_fitenss.csv");
    excelFile0 << "Sphere, Rastrigin, Schwefel, Simpleton30, Rosenbrock" << endl;
    excelFile1 << "Sphere, Rastrigin, Schwefel, Simpleton30, Rosenbrock" << endl;
*/
    double list_sphere[2][100];
    double list_rastrigin[2][100];
    double list_schwefel[2][100];
    double list_simpleton30[2][100];
    double list_rosenbrock[2][100];

    SFLA sfla_sphere(5, 10, 5, 10, 1);
    SFLA sfla_rastrigin(20, 20, 10, 10, 0.8);
    SFLA sfla_schwefel(10, 10, 8, 10, 1);
    SFLA sfla_simpleton30(15, 20, 15, 8, 0.2);
    SFLA sfla_rosenbrock(5, 5, 3, 10, 0.5);

    // Run algorithm
    SphereEqu sphereEqu(-100, 100, dim);
    Equation * sphere = &sphereEqu;
    run(sphere, &sfla_sphere, exp_time, computeNum, state, list_sphere);
    
    RastriginEqu rastriginEqu(-1.5, 1.5, dim);
    Equation * rastrigin = &rastriginEqu;
    run(rastrigin, &sfla_rastrigin, exp_time, computeNum, state, list_rastrigin);
    
    SchwefelEqu schwefelEqu(-500, 500, dim);
    Equation * schwefel = &schwefelEqu;
    run(schwefel, &sfla_schwefel, exp_time, computeNum, state, list_schwefel);

    SimpletonEqu simpleton30Equ(0, 10, dim);
    Equation * simpleton30 = &simpleton30Equ;
    run(simpleton30, &sfla_simpleton30, exp_time, computeNum, state, list_simpleton30);

    RosenbrockEqu rosenbrockEqu(-5, 10, dim);
    Equation * rosenbrock = &rosenbrockEqu;
    run(rosenbrock, &sfla_simpleton30, exp_time, computeNum, state, list_rosenbrock);
/*
    for (int j = 0; j < exp_time; j++){
        excelFile0 << to_string(list_sphere[0][j]) << ", " << to_string(list_rastrigin[0][j]) 
        << ", " << to_string(list_simpleton30[0][j]) << ", " << to_string(list_simpleton30[0][j]) << ", " << to_string(list_rosenbrock[0][j]) << endl;
    
        excelFile1 << to_string(list_sphere[1][j]) << ", " << to_string(list_rastrigin[1][j]) 
        << ", " << to_string(list_simpleton30[1][j]) << ", " << to_string(list_simpleton30[1][j]) << ", " << to_string(list_rosenbrock[1][j]) << endl;
    }
*/
    return 0;
}