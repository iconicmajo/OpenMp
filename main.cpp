/* 
*     Metrics:
*
*     This code is helpful to calculate the eficiency and speed up
*     of a parallel function
*
*/

#include "heatPDE_sec.cpp"  
#include "heatPDE_para.cpp"  
#include <chrono>
#include <iostream>
#include <iomanip>
using namespace std::chrono;
  
int THREAD_COUNT_PARALLEL = 5000;
double Tl = 0.0;
double Tr = 60.0;

void parallel_function(int THREAD_COUNT, double Tl, double Tr, double C, int x_intervals, int t_intervals){
    _Heat(THREAD_COUNT, Tl, Tr, C , x_intervals, t_intervals);
}

void sec_function(int THREAD_COUNT, double Tl, double Tr, double C, int x_intervals, int t_intervals){
    //The thread count is just used to set the 
    //number of intervals in the space dimension
    Heat(THREAD_COUNT, Tl, Tr, C , x_intervals, t_intervals);
}


int main(int argc, char const *argv[])
{
    try {
        THREAD_COUNT_PARALLEL = strtol(argv[1], NULL, 10);
        Tr = strtol(argv[2], NULL, 10);
        Tr = strtol(argv[3], NULL, 10);
    } catch (const std::exception&) {
        std::cout << "Hay algo mal el las entradas, se usaran los valores default" << std::endl;
    }

    //Discrete Intervals
    double C=0.4;
    int x_intervals = THREAD_COUNT_PARALLEL;
    int t_intervals = int((pow(x_intervals, 2) * 6e-3)/C);

    std::cout << "El valor de C es: " << C << std::endl;
    std::cout << "Se usaran : " << x_intervals << " intervalos en x" << std::endl;
    std::cout << "Se usaran : " << t_intervals << " intervalos en t" << std::endl;
    std::cout << "Tl: " << Tl << "°c" << std::endl;
    std::cout << "Tr: " << Tr << "°c\n" << std::endl;


    std::cout<<"Comenzando ejecución secuencial" << std::endl;
    auto initial_seq_time = high_resolution_clock::now();
    sec_function(THREAD_COUNT_PARALLEL, Tl, Tr, C , x_intervals, t_intervals);
    auto final_seq_time = high_resolution_clock::now();

    std::cout<<"Comenzando ejecución Paralela" << std::endl;
    auto initial_parallel_time = high_resolution_clock::now();
    parallel_function(THREAD_COUNT_PARALLEL, Tl, Tr, C , x_intervals, t_intervals);
    auto final_parallel_time = high_resolution_clock::now();

    auto seq_time = duration_cast<microseconds>(final_seq_time - initial_seq_time);
    auto parallel_time = duration_cast<microseconds>(final_parallel_time - initial_parallel_time);


    std::cout << "Tiempo Secuencial: " << seq_time.count() << " microsegundos" << std::endl;
    std::cout << "Tiempo Paralelo: " << parallel_time.count() << " microsegundos" << std::endl;

    long double speedup = seq_time.count() / parallel_time.count();
    long double eff = speedup / THREAD_COUNT_PARALLEL;
    std::cout << std::fixed << "Speed up: " << speedup << std::endl;
    std::cout << std::fixed << "Eficiencia: " << eff << std::endl;

    return 0;
}

/*
speed up = tiempo secuencial / tiempo paralelo 
eficiencia = speed up / threads
*/