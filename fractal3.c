#include <stdio.h>
#include <stdlib.h>
#include <math.h>




#define WIDTH		1024
#define HEIGHT		1024
#define NPOINTS		10000000
#define XMIN		-2.0
#define XMAX		1.0
#define YMIN		-1.5
#define YMAX		1.7
#define MAXITER		20
#define IMAGE(i,j)	image[WIDTH*(j) + (i)]

#define SIN60		0.866025403784439


typedef struct _complex_t {
	double real;
	double imag;
} complex_t;



void N (double *x, double *y, double cx, double cy);

double realN (double x, double y);
double imagN (double x, double y);
double norm (double x, double y);
char getColor (double x, double y);

void compAdd (complex_t *rop, complex_t op1, complex_t op2);
void compSub (complex_t *rop, complex_t op1, complex_t op2);
void compMul (complex_t *rop, complex_t op1, complex_t op2);
void compDiv (complex_t *rop, complex_t op1, complex_t op2);
void compPow (complex_t *rop, complex_t op1, int op2);


void getIJ (int *i, int *j, double x, double y) {
	*i = (int) floor ((x - XMIN) * (WIDTH - 1.0) / (XMAX - XMIN));
	*j = (int) floor ((y - YMIN) * (HEIGHT - 1.0) / (YMAX - YMIN));
}

double getRand (double a, double b) {
	return a + (((double) rand()) / RAND_MAX) * (b - a);
}


void main () {

	unsigned int *image;
	image = (unsigned int*) malloc (WIDTH * HEIGHT * sizeof (unsigned int));


	int i;		// COUNTER FOR WIDTH

#pragma omp parallel for
	for (i=0; i<NPOINTS; i++) {
		double cx = getRand (XMIN, XMAX); //XMIN + i * (XMAX-XMIN) / (WIDTH - 1.0);
		double cy = getRand (YMIN, YMAX); //YMIN + j * (YMAX-YMIN) / (HEIGHT - 1.0);
		double x = 0.0;
		double y = 0.0;
		unsigned int count = 0;
		do {
			if (count == 100000) break;
			count++;
			N (&x, &y, cx, cy);
			int I, J;
			getIJ (&I, &J, x, y);
			if (I>=0 && I < WIDTH && J>=0 && J < HEIGHT) 
				IMAGE(I,J)++;
		} while (norm (x, y) < 4.0);

	}

	int j;
	for (j=0; j<HEIGHT; j++) {	
		for (i=0; i<WIDTH; i++)
			printf ("%u  ", IMAGE(i,j));
		printf ("\n");
	}

	free (image);

}

void N (double *x, double *y, double cx, double cy) {
	// z^2-C
	complex_t z = {*x, *y};
	compMul (&z, z, z);
	*x = z.real + cx;
	*y = z.imag + cy;
}



double norm (double x, double y) {
	return sqrt (x*x+y*y);
}



void compAdd (complex_t *rop, complex_t op1, complex_t op2) {
	rop->real = op1.real + op2.real;
	rop->imag = op1.imag + op2.imag;
}

void compSub (complex_t *rop, complex_t op1, complex_t op2) {
	rop->real = op1.real - op2.real;
	rop->imag = op1.imag - op2.imag;
}

void compMul (complex_t *rop, complex_t op1, complex_t op2) {
	rop->real = op1.real*op2.real - op1.imag*op2.imag;
	rop->imag = op1.real*op2.imag + op1.imag*op2.real;
}

void compDiv (complex_t *rop, complex_t op1, complex_t op2) {
	double aux = op2.real*op2.real + op2.imag*op2.imag;
	rop->real = (op1.real*op2.real + op1.imag*op2.imag) / aux;
	rop->imag = (op1.imag*op2.real - op1.real*op2.imag) / aux;
}

void compPow (complex_t *rop, complex_t op1, int op2) {
	if (op2 == 0) {
		rop->real = 1.0;
		rop->imag = 0.0;
	} else if (op2 < 0) {
		rop->real = 1.0;
		rop->imag = 0.0;
		complex_t aux;
		compPow (&aux, op1, -op2);
		compDiv (rop, *rop, aux);
	} else {
		int i;
		rop->real = op1.real;
		rop->imag = op1.imag;
		for (i=1; i<op2; i++)
			compMul (rop, *rop, op1);
	}
		
}


