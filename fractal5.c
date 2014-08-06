#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define MVX		0.01
#define MVY		-0.015

#define MAXITER		80
#define WIDTH		1024
#define HEIGHT		1024
/*#define XMIN		-1.84
#define XMAX		-1.71
#define YMIN		-0.0775
#define YMAX		0.0475*/


#define XMIN		-1.82
#define XMAX		-1.54
#define YMIN		-0.2
#define YMAX		0.1


double norm (double x, double y) {
	return sqrt (x*x+y*y);
}


void main (int argc, char **argv) {

	if (argc > 1) {
		FILE *gnuplot = popen ("/usr/bin/gnuplot", "w");
		double dx = (XMAX - XMIN) / (WIDTH - 1.0);
		double dy = (YMAX - YMIN) / (HEIGHT - 1.0);
		fprintf (gnuplot, "set xrange [%f:%f]\n", XMIN, XMAX);
		fprintf (gnuplot, "set yrange [%f:%f]\n", YMIN, YMAX);
		fprintf (gnuplot, "unset key\n");
		fprintf (gnuplot, "set pm3d map\n");
		fprintf (gnuplot, "set palette rgbformulae 34,35,36\n");
		fprintf (gnuplot, "splot \"./fractal5.txt\" matrix u (%f+$1*%f):(%f+$2*%f):3\n", XMIN, dx, YMIN, dy);
		fflush (gnuplot);
		fgetc(stdin);
		fclose (gnuplot);
		return;
	}

	unsigned int *image;
	image = (unsigned int*) malloc (WIDTH * HEIGHT * sizeof (unsigned int));


	int i;		// COUNTER FOR WIDTH



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
			while (norm (zx, zy) < 80.0 && k < MAXITER) {
				temp = zx*zx - zy*zy + cx;
				zy = 2.0 * fabs(zx*zy) + cy;
				zx = temp;
				k++;
			}
			image[WIDTH*(HEIGHT-j-1) + i] = k;
		}
	}

	int j;
	for (j=0; j<HEIGHT; j++) {	
		for (i=0; i<WIDTH; i++)
			printf ("%le  ", 1.0 - (float) (image[WIDTH*j+i]) / MAXITER);
		printf ("\n");
	}

	free (image);

}


