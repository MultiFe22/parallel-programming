#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <time.h>
#include <math.h>
// block size, 128k is the sweet spot for Core i7 cpus
const int sliceSize = 128*1024;


int eratosthenes(long long int lastNumber);
int eratosthenesMP(long long int lastNumber);


int main(int argc, char const *argv[])
{
    printf("OpenMP uses up to %d threads running on %d processors\n",
        omp_get_max_threads(), omp_get_num_procs());
    long start, end;
    struct timeval timecheck;

    gettimeofday(&timecheck, NULL);
    start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;

    printf("%d\n", eratosthenesMP(10000000000));
    gettimeofday(&timecheck, NULL);
    end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;

    printf("%f seconds elapsed\n", (float)(end - start)/1000.0);
    return 0;
}


// simple serial sieve of Eratosthenes 
int eratosthenes(long long int lastNumber)
{
    // initialize
    char *isPrime = (char*)malloc(sizeof(char)*lastNumber);
    for (long long int i = 0; i <= lastNumber; i++)
        isPrime[i] = 1;
    // find all non-primes
    for (long long int i = 2; i*i <= lastNumber; i++)
        if (isPrime[i])
            for (long long int j = i*i; j <= lastNumber; j += i)
                isPrime[j] = 0;
    // sieve is complete, count primes
    int found = 0;
    for (long long int i = 2; i <= lastNumber; i++)
        found += isPrime[i];

    free(isPrime);
    


    return found;
}

// simple serial sieve of Eratosthenes 
int eratosthenesMP(long long int lastNumber)
{
    // initialize
    const int lastNumberSqrt = (int)sqrt((double)lastNumber);
    char *isPrime = (char*)malloc(sizeof(char)*lastNumber);
    #pragma omp parallel for
    for (long long int i = 0; i <= lastNumber; i++)
        isPrime[i] = 1;
    // find all non-primes
    #pragma omp parallel for schedule(dynamic)
    for (long long int i = 2; i <= lastNumberSqrt; i++)
        if (isPrime[i])
            for (long long int j = i*i; j <= lastNumber; j += i)
                isPrime[j] = 0;

    // sieve is complete, count primes
    int found = 0;
    #pragma omp parallel for reduction(+:found)
    for (long long int i = 2; i <= lastNumber; i++)
        found += isPrime[i];

    free(isPrime);
    


    return found;
}


