#include "functions.h"
//Невязка в первом инварианте
double GetErr1(int n,std::vector<std::vector<double>>& A,std::vector<double>& eigen){
    double rez = 0.0;
    for(int i = 0; i < n; i++){
        
        rez+=(A[i][i]-eigen[i]);
        
    }
    rez=fabs(rez);
    return rez;
}
//Невязка во втором инварианте
double GetErr2(int n,std::vector<std::vector<double>>& A,std::vector<double>& eigen){
    double rez = 0.0;
    double s1 = 0.0;
    double s2 = 0.0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n ; j++){
            s1+=(A[i][j]*A[i][j]);
        }
        s2+=(eigen[i]*eigen[i]);
    }
    s1=sqrt(s1);
    s2=sqrt(s2);
    rez=fabs(s1-s2);
    return rez;
}

