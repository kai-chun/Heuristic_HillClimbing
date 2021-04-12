#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;

int main(void){
    int pointNum = 300000; // number of random points
    int countCircle = 0; // count the points that location at quarter circle

    double rand_x, rand_y; // location (x, y) of random points
    double distance; // the distance of (0, 0) to (x, y)

    srand(time(NULL));
    
    for(int i = 0; i < pointNum; i++){
        rand_x = (double) rand() / (RAND_MAX + 1.0);
        rand_y = (double) rand() / (RAND_MAX + 1.0);
        
        distance = sqrt(pow(rand_x, 2.0) + pow(rand_y, 2.0));
        if (distance <= 1){
            countCircle++;
        }
    }

    double pi = (countCircle / (double)pointNum) * 4;
    cout << "PI = " << pi << endl;

    return 0;
}