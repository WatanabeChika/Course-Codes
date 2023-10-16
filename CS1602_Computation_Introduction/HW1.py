# Tower of Hanoi
# First Problem
def Hanoi(n,x,y,z):
    if n==1:
        print('{}->{}'.format(x,z))
    else:
        Hanoi(n-1,x,z,y)
        print('{}->{}'.format(x,z))
        Hanoi(n-1,y,x,z)
Hanoi(3,'A','B','C')

print()

# Second Problem
def Hanoi_plus(n,x,y,z):
    if n==1:
        print('{}->{}'.format(x,y))
        print('{}->{}'.format(y,z))
    else:
        Hanoi_plus(n-1,x,z,y)
        print('{}->{}'.format(x,y))
        Hanoi_plus(n-1,z,y,x)
        print('{}->{}'.format(y,z))
        Hanoi_plus(n-1,x,z,y)
Hanoi_plus(3,'A','B','C')

print()

# The Josephus Problem
# First Problem
def circle(x):
    list1=list(range(1,x+1))
    j=0
    while len(list1)>1:
        list2=list1.copy()
        for i in list2:
            if (list2.index(i)+1+j)%2==0:
                list1.remove(i)
        j+=len(list2)
    return list1[0]
print(circle(4))

print()

# Second Problem
def circle_1(n):
    if n<1:
        return False
    elif n==1:
        return 1
    elif n%2==0:
        return 2*circle_1(n/2)-1
    else:
        return 2*circle_1((n-1)/2)+1
print(circle_1(4))

print()

# Third Problem
def circle_2(y):
    for m in range(0,y):
        if 2**m<=y<2**(m+1):
            t=y-2**m
            break
    return 2*t+1
print(circle_2(4))