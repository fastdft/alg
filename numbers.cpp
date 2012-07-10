#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "numbers.h"

char is_prime_basic(int number)
{
    if (number < 2)
        return 0;
    if (number == 2)
        return 1;

    int sq = sqrt(number);
    int i;
    for (i = 2; i <= sq; i++)
    {
        if (number % i == 0)
            return 0;
    }

    return 1;
}

char is_prime_mr(int number)
{
    if (number < 2)
        return 0;
    if (number == 2 || number == 3 || number == 5 || number == 7 || number == 11 || number == 13)
        return 1;
    if (number % 2 == 0)
        return 0;

    int n, s, d;
    
    n = number - 1;
    for (s = 0; n % 2 == 0; s++, n >>= 1);
    d = n;

    for (int i = 0; i < 10; i++)
    {
        int a = rand() % (number - 1);
        if (a == 0)
            a = number - 2;
        assert(a != 0);

        int ret = 1;
        int j;

        for (j = 0; j < d; j++)
        {
            ret *= a;
            ret %= number;
        }
        if (ret == 1 || ret == number - 1)
            continue; 

        for (j = 0; j < s; j++)
        {
            ret = ret * ret % number;
            if (ret == 1)
                return 0;
            else if (ret == number - 1)
                break;
        }
        if (j == s)
            return 0;
    }

    return 1;
}

char is_prime_mr2(int number)
{
    if (number < 2)
        return 0;
    if (number == 2 || number == 3 || number == 5 || number == 7 || number == 11 || number == 13)
        return 1;
    if (number % 2 == 0)
        return 0;

    int s = floor(log(number - 1.0)/log(2.0));

    for (int i=0; i<10; i++)
    {
        int a = rand() % (number - 1);
        //int a = rand() * (number - 2) / RAND_MAX + 1;
        //printf("a, mr2: %d\n", a);
        if (a == 0)
            a = number - 2;
        assert(a != 0);

        int j=s, d=1;
        for (; j>=0; j--)
        {
            int x = d;
            d = d * d % number;
            if (d == 1 && x != 1 && x != number - 1)
                return false;
            if ((number - 1) & (1 << j))
                d = d * a % number;
        }
        if (d != 1)
            return false;
    }

    return 1;
}


char is_prime_test(int number)
{
    char ret1 = is_prime_basic(number);
    
    char ret2 = is_prime_mr(number);

    char ret3 = is_prime_mr2(number);

    if (ret1 != ret2 || ret1 != ret3)
        printf("prime test wrong: %d, ret1: %d, ret2: %d, ret3: %d\n", number, ret1, ret2, ret3);

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
        if (primes_size >= size)
            break;
    }

    return primes_size;
}

#define PRIMES_COUNT 10
void number_test()
{
    int primes[PRIMES_COUNT];
    int primes_number = get_primes_before(1<<30, primes, PRIMES_COUNT);
    printf("get_primes_before 100 is\n");
    for (int i=0; i<primes_number; i++)
        printf("%d\n", primes[i]);

    printf("prime number test\n");

    //srand((unsigned int)time(NULL));
    for (int i=0; i<10000; i++)
        is_prime_test(i);

}
