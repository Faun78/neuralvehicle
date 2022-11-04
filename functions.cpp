#include "matrix.cpp"
#include <random>
#include <ctime>
#include <math.h> 
using namespace std;
mt19937 mt3(time(0));
double λ=       1.0507009873554804934193349852946;
double aparam=  1.6732632423543772848170429916717;
double detanh(double x){
    return 1-tanh(x);
}
double sigmoid(double x){
    return (1/(1+exp(-x)));
}
double dsigmoid2(double y){
    return y*(1-y);
}
double dsigmoid(double x){
    return sigmoid(x)*(1-sigmoid(x));
}
double SeLU(double x){
    if(x<=0){
        return λ*aparam*(exp(x)-1);
    }
    return λ*x;
}
double deSeLU(double x){
    if(x<0){
        return λ*aparam*(exp(x));
    }
    return λ;
}
double randval(double min=-1.0,double max=1.0){
    static std::student_t_distribution<double> dis(max); // rage 0 - 1
    double answ=dis(mt3);
    if(int(answ)%2==0){
        answ=-answ;
    }

    return answ;
    }
double mutate3(double z,double rate){
    double ran =randval(0,1);
    if(ran<rate){
        return z+randval(-20,20);
    }else{
        return z;
    }
}