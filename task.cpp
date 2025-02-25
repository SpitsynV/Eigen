#include <math.h>

#include "task.h"

//Норма матрицы в l_inf
double GetNorm(int n, std::vector<std::vector<double>>& A)
{
	int i;
	int j;
	double tmp;
	double rez;

	rez = 0.0;
	for (i = 0; i < n; i++)
	{
		tmp = 0.0;
		for (j = 0; j < n; j++)
			tmp += fabs(A[i][j]);

		if (rez < tmp)
			rez = tmp;
	}

	return rez;
}

//Приведение к трехдиаг виду методом отражений
void Reflection(int n, std::vector<std::vector<double>>& A)
{
	int i;
	int j;
	int k;
	double tmp1;
	double tmp2;

	for (i = 0; i < n - 2; i++)
	{
		tmp1 = 0.0;
		for (j = i + 2; j < n; j++)
			tmp1 += A[j][i] * A[j][i];

		tmp2 = sqrt(A[i + 1][i] * A[i + 1][i] + tmp1);

		if (tmp2 < 1e-100)
		{
			A[i + 1][i] = 0.0;
			A[i + 2][i] = 0.0;

			continue;
		}

		if (tmp1 < 1e-100)
		{
			A[i + 2][i] = 0.0;

			continue;
		}

		A[i + 1][i] -= tmp2;

		tmp1 = 1.0 / sqrt(A[i + 1][i] * A[i + 1][i] + tmp1);
		for (j = i + 1; j < n; j++)
            A[j][i] *= tmp1;

		for (j = i + 1; j < n; j++)
		{
			tmp1 = 0.0;
			for (k = i + 1; k < n; k++)
				tmp1 += A[j][k] * A[k][i];
			A[i][j] = tmp1;
		}

		tmp1 = 0.0;
		for (j = i + 1; j < n; j++)
			tmp1 += A[i][j] * A[j][i];
		tmp1 *= 2.0;

		for (j = i + 1; j < n; j++)
			A[i][j] = 2.0 * A[i][j] - tmp1 * A[j][i];

		for (j = i + 1; j < n; j++)
			for (k = i + 1; k < n; k++)
                A[j][k] -= A[i][j] * A[k][i] + A[i][k] * A[j][i];

        A[i+1][i] = tmp2;
		A[i][i+1] = tmp2;
		for (j = i + 2; j < n; j++)
		{
			A[j][i] = 0.0;
			A[i][j] = 0.0;
		}
	}
}

double GetShift(int n,std::vector<std::vector<double>>& A, int k)
{
	return A[k-1][k];
}

void MakeShift(int n, std::vector<std::vector<double>>& A, int k, double s)
{
	int i;

	for (i = 0; i < k; i++)
		A[i][i]-= s;
}

void MakeDecomposition(int n, std::vector<std::vector<double>>& A, int k)
{
	int i;
	double tmp;

	for (i = 0; i < k - 1; i++)
	{
		tmp = A[i][i] - (i > 0 ? A[i][i-1] * A[i][i-1] : 0.0);

		if (tmp < 1e-100)
			tmp = 1e-10;

            A[i][i] = sqrt(tmp);

            A[i+1][i] /=A[i][i];
	}

	tmp = A[k-1][k-1] - A[k-1][k-2] * A[k-1][k-2];

	if (tmp < 1e-100)
		tmp = sqrt(tmp);

        A[k-1][k-1] = sqrt(tmp);
}

void GetProduct(int n, std::vector<std::vector<double>>& A, int k)
{
	int i;
	double tmp1;
	double tmp2;

	for (i = 0; i < k - 1; i++)
	{
		tmp1 = A[i][i];
		tmp2 = A[i+1][i];

		if (i != 0)
            A[i][i-1] *= tmp1;

        A[i][i] = tmp1 * tmp1 + tmp2 * tmp2;
		A[i][i+1] = tmp2 * A[i+1][i+1];
	}

	tmp1 = A[k-1][k-1];
	A[k-1][k-2] *= tmp1;
	A[k-1][k-1] *= tmp1;
}

int FindValues(int n,std::vector<std::vector<double>>& A, std::vector<double>& eigen, double eps)
{
	int i;
	int k;
	double t;
	double s;
	int iter=0;
	int alliter=0;
	t = eps/GetNorm(n, A);//по теореме
	Reflection(n, A);
	
	for (k = n; k > 2; --k)
	{
		iter=0;
		while (fabs(A[k-1][k-2]) > t)
		{
			s = GetShift(n, A, k);
			MakeShift(n, A, k, s);

			MakeDecomposition(n, A, k);
			GetProduct(n, A, k);

			MakeShift(n, A, k, -s);
			iter++;
		}
		alliter+=iter;
	}
	double tmp=0.0;
	if (n > 1)//двумерный случай
	{
		tmp = A[0][0] + A[1][1];
		s = A[0][0] * A[1][1] - A[0][1] * A[1][0];
		s = sqrt(tmp * tmp - 4.0 * s); 

		A[0][0] = 0.5 * (tmp + s);
		A[1][1] = 0.5 * (tmp - s);
	}

	for (i = 0; i < n; i++)
		eigen[i] = A[i][i];
	return alliter;
}
