#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define MAXITER		256
#define WIDTH		1024
#define HEIGHT		1024
#define XMIN		-2.0
#define XMAX		1.0
#define YMIN		-1.5
#define YMAX		1.7


double norm (double x, double y) {
	return sqrt (x*x+y*y);
}


void main () {

	unsigned int *image;
	image = (unsigned int*) malloc (WIDTH * HEIGHT * sizeof (unsigned int));


	int i;		// COUNTER FOR WIDTH

	int palette[256];

	for(i=0; i<256; i++) {
		palette[i] = (int) (i + 512 - 512 * exp (-i/50.0) / 3.0);
		palette[i] = palette[i]<<16 | palette[i]<<8 | palette[i];
	}

	palette[255]=0;


#pragma omp parallel for
	for (i=0; i<WIDTH; i++) {
		int j;				// COUNTER FOR HEIGHT
		unsigned int k;			// COUNTER FOR ITERATIONS
		double cx = XMIN + i * (XMAX-XMIN) / (WIDTH - 1.0);
		for (j=0; j<HEIGHT; j++) {
			double cy = YMIN + j * (YMAX-YMIN) / (HEIGHT - 1.0);
			double zx = 0.0;
			double zy = 0.0;
			double temp;
			k = 0;
			while (norm (zx, zy) < 4.0 && k < MAXITER) {
				temp = zx*zx - zy*zy + cx;
				zy = 2.0 * fabs(zx*zy) + cy;
				zx = temp;
				k++;
			}
			image[WIDTH*(HEIGHT-j-1) + i] = palette[k];
		}
	}

	int j;
	for (j=0; j<HEIGHT; j++) {	
		for (i=0; i<WIDTH; i++)
			printf ("%i  ", image[WIDTH*j + i] / MAXITER);
		printf ("\n");
	}

	free (image);

}


