#include "func.h"
#include "util.h"

void func0(double *weights, double *arrayX, double *arrayY, int xr, int yr, int n)
{
        int i;
        int a = 1/((double)(n));

        #pragma omp parallel for private(i)
	for(i = 0; i < n; i++){
	  weights[i] = a;
		arrayX[i] = xr;
		arrayY[i] = yr;
	}
}

void func1(int *seed, int *array, double *arrayX, double *arrayY, 
			double *probability, double *objxy, int *index, 
			int Ones, int iter, int X, int Y, int Z, int n)
{
  int i, j, a, b, c;
   	int index_X, index_Y;
	int max_size = X*Y*Z;

#pragma omp parallel for	
   	for(i = 0; i < n; i++){
	  arrayX[i] += 1 + 5*rand2(seed, i);
	  arrayY[i] += -2 + 2*rand2(seed, i);
   	}
   	
#pragma omp for private(j,index_X,index_Y)
   	for(i = 0; i<n; i++){
	
   		for(j = 0; j < Ones; j++){
		  a = i*Ones + j;
   			index_X = round(arrayX[i]) + objxy[j*2 + 1];
   			index_Y = round(arrayY[i]) + objxy[j*2];
   			index[a] = fabs(index_X*Y*Z + index_Y*Z + iter);
   			if(index[a] >= max_size) {
   				index[a] = 0;
			}
   		}
   		probability[i] = 0;
   		
   		for(j = 0; j < Ones; j++) {
		  probability[i] += (pow((array[index[i*Ones+j]] - 100),2) -
				     pow((array[index[i*Ones+j]]-228),2))/50.0;
   		}
   		probability[i] = probability[i]/((double) Ones);
   	}
   
}

void func2(double *weights, double *probability, int n) 
{
  int i;
	double sumWeights=0;
	
#pragma omp parallel for
	for(i = 0; i < n; i++)
   		weights[i] = weights[i] * exp(probability[i]);
	
#pragma omp nowait
#pragma omp for reduction (+:sumWeights)
   	for(i = 0; i < n; i++) {
   		sumWeights += weights[i];
	}
	
#pragma omp nowait
#pragma omp parallel for private(i)
	for(i = 0; i < n; i++)
   		weights[i] = weights[i]/sumWeights;

}

void func3(double *arrayX, double *arrayY, double *weights, double *x_e, double *y_e, int n)
{
	double estimate_x=0.0;
	double estimate_y=0.0;
	int i, a, temp1, temp2;
   	
#pragma omp parallel for private(temp1, temp2)
	for(i = 0; i < n; i++){
	  a = weights[i];
	  temp1 = arrayX[i] * a;
	  temp2 = arrayY[i] * a;
          #pragma omp critical
   		estimate_x += temp1;
          #pragma omp critical
   		estimate_y += temp2;
   	}

	*x_e = estimate_x;
	*y_e = estimate_y;

}

void func4(double *u, double u1, int n) 
{
  int i;
  int a = (double)n;
   	
#pragma omp parallel for private(i)
	for(i = 0; i < n; i++){
   		u[i] = u1 + i/a;
   	}
}

void func5(double *x_j, double *y_j, double *arrayX, double *arrayY, double *weights, double *cfd, double *u, int n)
{
	int i, j;
	int a = 1/((double)(n));


#pragma omp parallel for   	
	for(j = 0; j < n; j++){
   		//i = findIndex(cfd, n, u[j]);
   		i = findIndexBin(cfd, 0, n, u[j]);
   		if(i == -1)
   			i = n-1;
   		x_j[j] = arrayX[i];
   		y_j[j] = arrayY[i];
   		
   	}
	       
#pragma omp parallel for
	for(i = 0; i < n; i++){
		arrayX[i] = x_j[i];
		arrayY[i] = y_j[i];
		weights[i] = a;
	}
}
