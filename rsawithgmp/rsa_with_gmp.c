#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#define BITSTRENGTH 1024
int main()
{

    //scanf("%1024s",inp);
    mpz_t p,q,e,d,p1,q1,n,phi;
    mpz_init(p);mpz_init(p1);
    mpz_init(q);mpz_init(q1);
    mpz_init(e);
    //mpz_set_ui(e, 17);
    mpz_set_ui(e, 3);

    //  mpz_set_ui(p1,564654651549685465161654);
    //  mpz_set_ui(q1,154981687198787198719287);
    //mpz_set_ui(e, 65536);
    char inp1[BITSTRENGTH+1]="564654651549685465161654";
    mpz_set_str(p1,inp1,10);
    char inp2[BITSTRENGTH+1]="154981687198787198719287";
    mpz_set_str(q1,inp2,10);
   // printf("\nEnter the value of p",mpz_get_str())


    mpz_nextprime(p,p1);
    mpz_nextprime(q,q1);
    mpz_init(n);
    mpz_mul(n,p,q);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi,p1,q1);
    mpz_invert(d,e,phi);

    mpz_t M;  mpz_init(M);
    mpz_t C;  mpz_init(C);
    mpz_t Dc; mpz_init(Dc);

     char inp3[BITSTRENGTH+1]="9546584615";
    mpz_set_str(M,inp3,10);
    /* Encryption  */
    mpz_powm(C,M,e,n);
    printf("Encrypted Message is :  %s\n",mpz_get_str(NULL,10,C));

    /* Decryption  */
    mpz_powm(Dc,C,d,n);
    printf("Decrypted Message is :  %s\n",mpz_get_str(NULL,10,Dc));

    mpz_clear(p);mpz_clear(q);mpz_clear(e);mpz_clear(d);mpz_clear(p1);mpz_clear(q1);mpz_clear(n);mpz_clear(phi);
    return 0;
}

