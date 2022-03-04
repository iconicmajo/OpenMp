#include <stdio.h>
#include <math.h>
#include "DynamicMatrix.cpp"  

void Heat(int intervals);

bool stopCondition(double new_value, double old_value){
    return (abs(new_value-old_value) < 1e-240);
}
    
int main(int argc, char* argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);
    Heat(thread_count);
    return 0; 
}

void Heat(int intervals) {

    //Frontier conditions
    double Tl = 20.0;
    double T0 = 1e-2;
    double Tr = 60.0;

    //Discrete Intervals
    double C=0.1;
    int x_intervals = intervals;
    int t_intervals = int((pow(x_intervals, 2) * 1e-5)/C);
    DynamicMatrix solution = DynamicMatrix();
    solution.columns = t_intervals;
    solution.rows = x_intervals;
    solution.matrix = new double[x_intervals * t_intervals];

    //Initial Values
    int i = 0;

    bool done = false;

    while (!done){
        for (int j = 1; j < x_intervals - 1; j++){
            double T_ji;   //T(j,i)
            double T_J1i;  //T(j+1,i)
            double T_j1i;  //T(j-1,i)
            
            if(j-1==0){
                solution.set(j - 1, i, Tl);
            } if(j+1==x_intervals-1){
                solution.set(j + 1, i, Tr);
            }

            if(i!=0){
                T_ji = solution.get(j,i);
                T_J1i =  solution.get(j+1,i);
                T_j1i = solution.get(j-1,i);
            } else {
                T_ji = T0 ;
                T_J1i = j-1==0 ? Tl : T0;
                T_j1i = j+1==x_intervals-1 ? Tr : T0;
                solution.set(j , i, T0);
            }

            double new_value = T_ji + C*(T_j1i - 2*T_ji + T_J1i);
            printf("i: %d, j:%d\n", i, j);
            if(i!=0 && j-1!=0 && j+1!=x_intervals-1){
                if(stopCondition(new_value, solution.get(j, i-1))){
                    done = true;
                }
            }
            solution.set(j , i+1, new_value);
        }
        i++;
    }  
}

  
 