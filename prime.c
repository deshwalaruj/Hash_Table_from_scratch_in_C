#include <math.h>
#include "prime.h"


int is_prime(int num) { // return 1 for prime and 0 for not prime
    for (int i = 2; (i * i) <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;

}

int next_prime(int num) {
    while (is_prime(num) != 1) {
        num++;
    }
    return num;
}