#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_N	4
#define P(i,j)		points[2*(i)+(j)]

int ipow (int n) {
	if (n == 0) return 1;
	return 2 * ipow (n-1);
}

int main (int argc, char **argv) {

	int n;				// NUMBER OF ITERATIONS
	if (argc > 1) 
		n = atoi (argv[1]);
	else 
		n = DEFAULT_N;

	int nfolds = ipow (n) - 1;
	int npoints = nfolds + 2;


	char *folds = (char*) malloc (nfolds * sizeof (char));
	double *points = (double*) malloc (npoints * 2 * sizeof (double));
			P(0,0) = P(0,1) = P(1,0) = 0.0;
			P(1,1) = 1.0;
	double v[2];

	int i;				// COUNTER FOR INTERATIONS
	int j;				// COUNTER FOR FOLDS IN EACH ITERATION

	folds[nfolds-1] = 1;
	int initPos;
	int increment;

	initPos = nfolds/2;
	increment = nfolds + 1;
	for (i=0; i<n; i++) {
		for (j=0; initPos + j * increment < nfolds; j++)
			if (j%2 == 0)
				folds[initPos + j * increment] = 1;
			else
				folds[initPos + j * increment] = 0;
		initPos /= 2;
		increment /= 2;
	}
		


	for (i=0; i<nfolds; i++) {
		v[0] = P(i+1,1) - P(i,1);
		v[1] = -P(i+1,0) + P(i,0);
		if (folds[i] == 0) {
			v[0] = -v[0];
			v[1] = -v[1];
		}
		P(i+2,0) = P(i+1,0) + v[0];
		P(i+2,1) = P(i+1,1) + v[1];	
	}


	for (i=0; i<npoints; i++)
		printf ("% lf   % lf\n", P(i,0), P(i,1));
	printf ("\n");	

	free (folds);
	free (points);



}

