#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
unsigned s[168] = {2,3,5,7,11,13,17,19,23,29,
31,37,41,43,47,53,59,61,67,71,73,79,83,
89,97,101,103,107,109,113,127,131,137,139,
149,151,157,163,167,173,179,181,191,193,197,
199,211,223,227,229,233,239,241,251,257,263,269,
271,277,281,283,293,307,311,313,317,331,337,347,349,
353,359,367,373,379,383,389,397,401,409,419,421,431,433,
439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,
541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,
641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,
751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,
877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};

int main() {
    int n;
    mpz_t e,p,q,rop,p1,q1,rop1;
    mpz_init(e);
    mpz_init(p);
    mpz_init(p1);
    mpz_init(q1);
    mpz_init(q);
    mpz_init(rop);
    mpz_init(rop1);
    scanf("%d",&n);
    while (n--) {
        gmp_scanf("%Zd",e);
        gmp_scanf("%Zd",p);
        gmp_scanf("%Zd",q);
        mpz_sub_ui(p1,p,1); //p1 = p-1
        mpz_sub_ui(q1,q,1); //q1 = q-1
        if (mpz_cmp_si(e,65536) < 0) {  //e >65536
            printf("ERROR\n");
            continue;
        }
        if (mpz_probab_prime_p(p,20) == 0 || mpz_probab_prime_p(q,20) == 0) { //p,q是否为素数
            printf("ERROR\n");
            continue;
        }
        mpz_sub(rop,p,q);
        mpz_abs(rop,rop);
        if (mpz_cmp_si(rop,65536) < 0) {  //65536>p-q>-65536
            printf("ERROR\n");
            continue;
        }
        /*mpz_gcd(rop,p1,q1);
        if (mpz_cmp_si(rop,14) <0) { //gcd(p-1,q-1) < 20
            printf("ERROR\n");
            continue;
        }*/
        mpz_t r,t;
        mpz_inits(t,r,NULL);
        mpz_sub_ui(t,p,1);
        for(int i=0;i<168;i++){
            mpz_fdiv_r_ui(r,t,s[i]);
            while(mpz_cmp_si(r,0)==0){
                mpz_fdiv_q_ui(t,t,s[i]);
                mpz_fdiv_r_ui(r,t,s[i]);
            }
        }
        if (mpz_cmp_si(t,1)==0){
            printf("ERROR\n");
            continue;
        }
        mpz_mul(rop1,p1,q1);
        mpz_gcd(rop,rop1,e);
        if (mpz_cmp_ui(rop,1) !=0) { //e 和 fi(n)不互质 或 e >fi(n)
            printf("ERROR\n");
            continue;
        }
        mpz_invert(rop,e,rop1); // d= e % fi(n);
        gmp_printf("%Zd\n",rop);
    }
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(p1);
    mpz_clear(q1);
    mpz_clear(rop);
    mpz_clear(rop1);

    return 0;
}