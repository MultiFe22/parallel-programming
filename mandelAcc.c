#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define		X_RESN	800       /* x resolution */
#define		Y_RESN	800       /* y resolution */

typedef struct complextype
	{
        float real, imag;
	} Compl;


void main ()
{
    /* Mandlebrot variables */
    int i, j, k;
    Compl	z, c;
    float	lengthsq, temp;
    int counter = 0;

    long start, end;
    struct timeval timecheck;

    gettimeofday(&timecheck, NULL);
    start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;

        /* Calculate and draw points */
    #pragma acc parallel loop copy(counter) collapse(2)
    for(i=0; i < X_RESN; i++) 
        for(j=0; j < Y_RESN; j++) {

            z.real = z.imag = 0.0;
            c.real = ((float) j - 400.0)/200.0;               /* scale factors for 800 x 800 window */
            c.imag = ((float) i - 400.0)/200.0;
            k = 0;

            do  {                                             /* iterate for pixel color */

                temp = z.real*z.real - z.imag*z.imag + c.real;
                z.imag = 2.0*z.real*z.imag + c.imag;
                z.real = temp;
                lengthsq = z.real*z.real+z.imag*z.imag;
                k++;

            } while (lengthsq < 4.0 && k < 100000);
             
            if (k == 100000){
                #pragma acc atomic update
                counter++;
            }
        }
	gettimeofday(&timecheck, NULL);
    end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
    printf("Counter: %d\n",counter); 
    printf("%f seconds elapsed\n", (float)(end - start)/1000.0);

	/* Program Finished */

}
