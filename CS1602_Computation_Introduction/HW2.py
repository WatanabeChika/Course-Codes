import time

# Problem Description (prime number)

# Count primes
# 1. Brute Force
def BF(x):
    primes=list(range(2,x+1))
    for i in primes:
        for j in range(2,i):
            if i%j==0:
                primes[primes.index(i)]=-1
                break
    return len(primes)-primes.count(-1)

# 2. Optimize Brute Force
def OBF(x):
    primes=[1]*(x-1)
    for i in range(2,x+1):
        for j in range(2,int(i**(0.5))+1):
            if i%j==0:
                primes.pop()
                break
    return len(primes)

# 3. Optimize Factor
def OF(x):
    primes=[1,0]*(x//6)
    for i in list(range(5,x,6))+list(range(7,x,6)):
        for j in range(2,int(i**(0.5))+1):
            if i%j==0:
                primes.pop()
                break
    return len(primes)+2

# 4. Sieve of Eratosthenes
def SE(x):
    primes=[1]*(x-1)
    for i in range(2,int(x**(0.5))+1):
        if primes[i-2]!=-1:
            for j in range(i,x//i+1):
                primes[i*j-2]=-1
    return len(primes)-primes.count(-1)

# Determine a prime
# 5. Miller-Rabin  #分解成几个小函数
def MR(n):
    if n<8:
        if n==4 or n==6:
            return "Not prime"
        else:
            return 'Is prime'
    number=[]
    for a in [2,3,5,7]:
        u=0
        t=0
        m=1
        for i in range(n,1,-1):
            if (n-1)%(2**i)==0:
                u=(n-1)//(2**i)
                t=i
                break
        if u==0:
            u=n-1
            y=a**u
        else:
            y=a**u
            while m<t:
                y=y**2
                m+=1
        if (y**2)%n==1:
            number.append(n)
    if len(number)==4:
        return 'Is prime'
    else:
        return "Not prime"


def PN_X(n,m):
    time_start=time.time()
    number_pn=0
    number_pn=m(n)
    time_end=time.time()
    return time_end-time_start, number_pn

print('(1-10000)Brute Force: {}'.format(PN_X(10000,BF)))
print('(1-1000000)Optimize Brute Force: {}'.format(PN_X(1000000,OBF)))
print('(1-1000000)Optimize Factor: {}'.format(PN_X(1000000,OF)))
print('(1-1000000)Sieve of Eratosthenes: {}'.format(PN_X(1000000,SE)))
print('Miller-Rabin: {}'.format(PN_X(49999,MR)))