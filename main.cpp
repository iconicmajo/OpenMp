/* 
*     Metrics:
*
*     This code is helpful to calculate the eficiency and speed up
*     of a parallel function
*
*/

#include "heatPDE_sec.cpp"  
#include <chrono>
#include <iostream>
#include <iomanip>
using namespace std::chrono;
  
int THREAD_COUNT_PARALLEL = 5000;

void parallel_function(int THREAD_COUNT){
    Heat(THREAD_COUNT);
}

int main(int argc, char const *argv[])
{
    std::cout<<"Comenzando" << std::endl;
    auto initial_seq_time = high_resolution_clock::now();
    parallel_function(THREAD_COUNT_PARALLEL);
    auto final_seq_time = high_resolution_clock::now();

    std::cout<<"Termino el primero" << std::endl;
    auto initial_parallel_time = high_resolution_clock::now();
    parallel_function(THREAD_COUNT_PARALLEL);
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