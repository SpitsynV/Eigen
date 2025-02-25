#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<fstream>
#include<ctime>
#include <iomanip>
using namespace std;

vector<vector<double>> generate_random_matrix(int n) {
    vector<vector<double>> matrix(n, vector<double>(n));
    srand(time(0));

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            matrix[i][j] = static_cast<double>(std::rand()) / RAND_MAX * 20.0 - 10.0;  // Random number between -10 and 10
        }
    }
    return matrix;
}
void make_symmetric(vector<vector<double>>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            matrix[j][i] = matrix[i][j];  
        }
    }
}

void make_positive_definite(vector<vector<double>>& matrix, int n) {
    double scalar = 10*n+1;

    for (int i = 0; i < n; i++) {
        matrix[i][i] += scalar;  // Add the scalar to the diagonal elements to ensure positive defin
    }
}
void write_matrix_to_file(const vector<vector<double>>& matrix, int n, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << setw(10) << matrix[i][j] << " ";  // Formatting similar to console output
        }
        file << endl;
    }

    file.close();  
    cout << "Matrix written to " << filename << endl;
}
int main(int argc, char* argv[]){
    srand(time(0));
    int n=stoi(argv[1]);
    vector<vector<double>> A;
    A.resize(n, vector<double>(n));
    A=generate_random_matrix(n);
    make_symmetric(A,n);
    make_positive_definite(A,n);
    write_matrix_to_file(A, n, "input.txt");
}