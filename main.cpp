//основная программа ищем собств значения, считаем ошибку
#include <iostream>
#include <chrono>
#include "matrix.h"
#include "functions.h"
#include "task.h"



int main(int argc, char* argv[]) {
    if (argc < 5 || argc > 6) {
        std::cerr << "Usage: " << argv[0] << " n m eps k filename" << std::endl; //проверь формат
        return 1;
    }

    int n = std::stoi(argv[1]);       // matrix dimension
    int m = std::stoi(argv[2]);       // amount of output values
    double eps = std::stod(argv[3]);  
    int k = std::stoi(argv[4]);       // formula number
    std::string filename="";
    if (m > n) {
        std::cerr << "Error:Number of values to be output is greater than the matrix dimension " << std::endl;
        return 1;
    }
 //----//
    int err = 0;
    std::vector<std::vector<double>> A;
    std::vector<double> eigen;
    A.resize(n, std::vector<double>(n));
    eigen.resize(n);
    if (k == 0) {
        filename = argv[5];
        err = readMatrixFromFile(filename, A, n);
        if (!err) {
            std::cerr << "Error: Can't read from file " << filename << std::endl;
            return err;
        }
    }
    else {
        initializeMatrix(A, k, n);
    }

    std::cout << "Initial matrix A:" << std::endl;
    printMatrix(A, m);



    std::chrono::duration<double> elapsed;
    int iter=0;
    try {
        auto start = std::chrono::high_resolution_clock::now();
        iter=FindValues(n,A,eigen,eps);
        auto end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Eigen values: "<<std::endl;
        printVector(eigen,n,(n>10?10:n));

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() <<std::endl;
        return 0;
    }
    // time
    std::cout << "Time to solve: " << elapsed.count() << " ticks" << std::endl;
    std::cout << "Iterations: " << iter<< std::endl;


//Подсчет норм
    if(k==0){
        readMatrixFromFile(filename, A, n);
    }else{
        initializeMatrix(A, k, n);
    }
    double e1 = 0.0;
    double e2 = 0.0;
    e1 = GetErr1(n,A,eigen);
    e2 = GetErr2(n,A,eigen);
    // results

    std::cout << "Норма невязки 1: " << std::scientific << e1 << std::endl;
    std::cout << "Норма невязки 2: " << std::scientific << e2 << std::endl;
    return 0;
}
