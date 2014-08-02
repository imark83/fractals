#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double realN (double x, double y);
double imagN (double x, double y);
double norm (double x, double y);
char getColor (double x, double y);


#define WIDTH		1024
#define HEIGHT		1024
#define XMIN		-2.0
#define XMAX		2.0
#define YMIN		-2.0
#define YMAX		2.0
#define MAXITER		20
#define IMAGE(i,j)	image[WIDTH*(j) + (i)]

#define SIN60		0.866025403784439


void main () {
	char *image;
	image = (char*) malloc (WIDTH * HEIGHT * sizeof (char));


	int i;		// COUNTER FOR WIDTH

#pragma omp parallel for
	for (i=0; i<WIDTH; i++) {
		int j;	// COUNTER FOR HEIGHT
		int k;	// COUNTER FOR NEWTON ITERATION
		for (j=0; j<HEIGHT; j++) {
			double aux_real;
			double x = XMIN + i * (XMAX-XMIN) / (WIDTH - 1.0);
			double y = YMIN + j * (YMAX-YMIN) / (HEIGHT - 1.0);
			int count = 0;
			do {
				if (count == 9) break;
				count++;
				aux_real = realN (x, y);
				y = imagN (x, y);
				x = aux_real;
			} while (!getColor (x,y));
			IMAGE(i,j) = getColor (x,y);
		}

	}

	int j;
	for (j=0; j<HEIGHT; j++) {	
		for (i=0; i<WIDTH; i++)
			printf ("%hhi  ", IMAGE(i,j));
		printf ("\n");
	}

	free (image);

}

double norm (double x, double y) {
	return sqrt (x*x+y*y);
}

char getColor (double x, double y) {
	if (norm (x, y) < 0.001) return 1;
	if (norm (x+0.5, y-SIN60) < 0.001) return 2;
	if (norm (x+0.5, y+SIN60) < 0.001) return 3;
	if (norm (x-1.0, y) < 0.001) return 4;

	return 0;

}





double realN (double x, double y) {
	//return x - ((x*x-y*y)*(x*x*x-3.0*x*y*y-1.0) + 2.0*x*y*(3*x*x*y-y*y*y)) / (3.0*(x*x+y*y)*(x*x+y*y));
	return 20*pow(y,6)*x/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 60*pow(y,4)*pow(x,3)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 60*pow(y,2)*pow(x,5)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 20*pow(x,7)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) - 3*pow(y,4)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 48*pow(y,2)*pow(x,2)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) - 13*pow(x,4)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 2*x/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4);
}

double imagN (double x, double y) {
	//return y - ((x*x-y*y)*(3*x*x*y-y*y*y) - 2.0*x*y*(x*x*x-3.0*x*y*y-1.0)) / (3.0*(x*x+y*y)*(x*x+y*y));
	return 20*pow(y,7)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 60*pow(y,5)*pow(x,2)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 60*pow(y,3)*pow(x,4)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 20*y*pow(x,6)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 42*pow(y,3)*x/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) - 22*y*pow(x,3)/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4) + 2*y/(25*pow(y,6) + 75*pow(y,4)*pow(x,2) + 75*pow(y,2)*pow(x,4) + 25*pow(x,6) + 60*pow(y,2)*x - 20*pow(x,3) + 4);
}
