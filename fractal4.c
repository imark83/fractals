#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_N	4

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

	int nfolds = ipow (n+1) - 1;
	int npoints = nfolds + 1;
	printf ("n = %i\tnfolds = %i\n", n, nfolds);
	char *folds = (char*) malloc (nfolds * sizeof (char));

	int i;				// COUNTER FOR INTERATIONS
	int j;				// COUNTER FOR FOLDS IN EACH ITERATION

	folds[nfolds/2] = 1;b ºfbhhfghhgjb,



	free (folds);



}

