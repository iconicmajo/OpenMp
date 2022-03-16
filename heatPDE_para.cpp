#include <stdio.h>
#include <math.h>

//already included in secuential algorithm
//#include "DynamicMatrix.cpp"  

void _Heat(int intervals);

double _T0(int j, double Tr,double Tl, int N){
    return (Tl-Tr)/(j*N);
}

bool _stopCondition(double i){
    return (i>100000);
}

void _iteration(int i, DynamicMatrix solution,double Tl,double Tr, int x_intervals,double C){
    #pragma omp parallel shared(solution, Tl, Tr, x_intervals, C)
    #pragma omp single nowait
    {
        for (int j = 1; j < x_intervals - 1; j++){
            #pragma omp task
                //Asignation of frontiers can be made in another thread
                if(j-1==0){
                    solution.set(j - 1, i, Tl);
                } if(j+1==x_intervals-1){
                    solution.set(j + 1, i, Tr);
                }

                double T_ji;   //T(j,i)
                double T_J1i;  //T(j+1,i)
                double T_j1i;  //T(j-1,i)
                if(i!=0){
                    T_ji = solution.get(j,i);
                    T_J1i =  solution.get(j+1,i);
                    T_j1i = solution.get(j-1,i);
                } else {
                    T_ji = T0(j, Tl, Tr, x_intervals);
                    T_J1i = T0(j+1, Tl, Tr, x_intervals);
                    T_j1i = T0(j-1, Tl, Tr, x_intervals);
                    solution.set(j , i, T0(j, Tl, Tr, x_intervals));
                }

                double new_value = T_ji + C*(T_j1i - 2*T_ji + T_J1i);
                //printf("i: %d\n", i);//, j:%d, value:%f\n", i, j, new_value);
                if(i!=0 && j-1!=0 && j+1!=x_intervals-1){
                    if(stopCondition(i)){
                        continue;
                    } else {
                        _iteration(i+1, solution, Tl, Tr, x_intervals, C);
                    }
                }
                solution.set(j , i+1, new_value);
        }
    }
}

void _Heat(int intervals) {
    //Frontier conditions
    double Tl = 0.0;
    double Tr = 60.0;

    //Discrete Intervals
    double C=0.4;
    int x_intervals = intervals;
    int t_intervals = int((pow(x_intervals, 2) * 6e-3)/C);
    DynamicMatrix solution = DynamicMatrix();
    solution.columns = t_intervals;
    solution.rows = x_intervals;
    solution.matrix = new double[x_intervals * t_intervals];

    //Initial Values
    _iteration(0, solution, Tl, Tr, x_intervals, C);
}

  
 