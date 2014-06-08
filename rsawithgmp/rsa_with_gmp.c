#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#define BITSTRENGTH 1024
int main()
{
    mpz_t p,q,e,d,p1,q1,n,phi;
    mpz_init(p);mpz_init(p1);
    mpz_init(q);mpz_init(q1);
    mpz_init(e);
    //mpz_set_ui(e, 17);
    //mpz_set_ui(e, 3);
    mpz_set_ui(e, 65536);
    mpz_set_ui(p1,5646546515564894968546516184165416541654165716546541654);
    mpz_set_ui(q1,1549879465168719871984254654197871987198709809898719287);
    mpz_nextprime(p,p1);
    mpz_nextprime(q,q1);
    mpz_init(n);
    mpz_mul(n,p,q);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q2, q, 1);
    mpz_mul(phi,p1,q1);
    mpz_invert(d,e,phi);

    mpz_t M;  mpz_init(M);
    mpz_t C;  mpz_init(C);
    mpz_t Dc; mpz_init(Dc);

    /* Encryption  */
    mpz_powm(C,M,e,n);

    /* Decryption  */
    mpz_powm(Dc,C,d,n);

    mpz_clear(p);mpz_clear(q);mpz_clear(e);mpz_clear(d);mpz_clear(p1);mpz_clear(q1);mpz_clear(n);mpz_clear(phi);
    return 0;
}

