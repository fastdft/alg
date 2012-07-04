#include <stdio.h>
#include <math.h>
#include "numbers.h"

char is_prime_basic(int number)
{
    int sq = sqrt(number);
    int i;
    for (i = 2; i <= sq; i++)
    {
        if (number % i == 0)
            return 0;
    }

    return 1;
}

char is_prime(int number)
{
    return 0;
}

int get_primes_before(int max_number, int primes[], int size)
{
    int primes_size = 0;
    int i, digital;

    if (max_number <= 1)
        return 0;

    primes[0] = 2;
    primes_size = 1;

    for (digital = 3; digital < max_number; digital++)
    {
        for (i=0; i<primes_size; i++)
        {
            if (digital % primes[i] == 0)
                break;
        }
        if (i == primes_size)
            primes[primes_size++] = digital;
    }

    return primes_size;
}
