#include <iostream>
#include <vector>
using namespace std;
#include <stdlib.h>
#include <math.h>

void TrivalDivision(long maxn)
{
    vector<long> primes;
    primes.push_back(2);
    cout << 2 << endl;
    for(long i = 2; i < maxn; i ++)
    {
        bool isprime = true;
        for(size_t j = 0; j < primes.size(); j ++)
        {
            if(i % primes[j] == 0)
            {
                isprime = false;
                break;
            }
        }
        if(isprime)
        {
            cout << i << endl;
            primes.push_back(i);
        }
    }
}

void Sieve(long maxn)
{
    vector<bool> isprime(maxn + 1, true);
    isprime[0] = false;
    isprime[1] = false;
    long prime = 2;
    long sqrtn = sqrt(maxn);
    while(prime < maxn)
    {
        cout << prime << endl;
        if(prime <= sqrtn)
        {
            for(long i = prime*2; i < maxn; i += prime)
                isprime[i] = false;
        }
        do {
            prime ++;
        } while(!isprime[prime]);
    }
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " maxn" << endl;
        exit(-1);
    }
    long maxn = atol(argv[1]);
    //TrivalDivision(maxn);
    Sieve(maxn);
    return 0;
}
