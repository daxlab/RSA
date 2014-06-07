#include <stdio.h>
#define ll long long
#define s(n) scanf("%lld",&n)
/*  Fermat's Little Theorem  */
ll pow(ll a, ll b, ll MOD) {
ll x = 1, y = a;
	while(b > 0) {
		if(b%2 == 1) {
			x=(x*y);
			if(x>MOD) x%=MOD;
		}
		y = (y*y);
		if(y>MOD) y%=MOD;
		b=b>>1;
	}
	return x;
}

ll modInverse(ll a, ll m) {
    return pow(a,m-2,m);
}

ll mul_inv(ll a, ll b)
{
	ll b0 = b, t, q;
	ll x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}


/*  end    */



int main()
{
    ll p,q,e=17,d,phi;
    s(p);
    s(q);
    phi=(p-1)*(q-1);

    d=mul_inv(e,phi);
    printf("%lld",d);

   return 0;
}
