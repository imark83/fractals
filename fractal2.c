#include <stdio.h>
#include <stdlib.h>
#include <math.h>




#define WIDTH		1024
#define HEIGHT		1024
#define XMIN		-2.0
#define XMAX		2.0
#define YMIN		-2.0
#define YMAX		2.0
#define MAXITER		20
#define IMAGE(i,j)	image[WIDTH*(j) + (i)]

#define SIN60		0.866025403784439



void N (double *x, double *y);

double realN (double x, double y);
double imagN (double x, double y);
double norm (double x, double y);
char getColor (double x, double y);

void compAdd (complex_t *rop, complex_t op1, complex_t op2);
void compSub (complex_t *rop, complex_t op1, complex_t op2);
void compMul (complex_t *rop, complex_t op1, complex_t op2);
void compDiv (complex_t *rop, complex_t op1, complex_t op2);
void compPow (complex_t *rop, complex_t op1, int op2);

typedef struct _complex_t {
	double real;
	double imag;
} complex_t;




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
				N (&x, &y);
			} while (count < 10);
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

void N (double *x, double *y) {
	// 	9*z^4/(5*z^6 + 2*z^3 + 2)
	complex_t z = {*x, *y};
	complex_t z3;
		compPow (&z3, z, 3);
	complex_t z4;
		compMul (&z4, z3, z);
	complex_t z6;
		compMul (&z6, z3, z3);
	complex_t rop;
		rop.real = 5.0*z6.real + 2.0*z3.real + 2.0;
		rop.imag = 5.0*z6.imag + 2.0*z3.imag;
		compDiv (&rop, z4, rop);
	*x = 9.0*rop.real;
	*y = 9.0*rop.imag;
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


