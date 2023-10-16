
def str2list(x):
    if x[0]!='-':
        y=list(x)
        for i in y:
            y[y.index(i)]=int(i)
        return y
    else:
        y=list(x[1:len(x)])
        for i in y:
            y[y.index(i)]=-int(i)
        return y

def list2str(x):
    if x[0]>=0:
        for i in x:
            x[x.index(i)]=str(i)
        return ''.join(x)
    else:
        for i in x:
            x[x.index(i)]=str(-i)
        y=['-']+x
        return ''.join(y)

def add(x,y):
    i=x[::-1]
    j=y[::-1]
    k=[]
    dist=len(i)-len(j)
    while len(i)!=len(j):
        if len(i)>len(j):
            i.pop()
        else:
            j.pop()
    for m,n in zip(i,j):
        k.append(m+n)
    if dist>0:
        for a in range(dist-1,-1,-1):
            k.append(x[a])
    elif dist<0:
        for b in range(-dist-1,-1,-1):
            k.append(y[b])
    if k[-1]>0:
        for c in range(len(k)):
            if k[c]>=10:
                if c!=len(k)-1:
                    k[c+1]+=k[c]//10
                    k[c]%=10
                else:
                    k.append(k[c]//10)
                    k[c]%=10
        for d in range(len(k)):
            if k[d]<0:
                k[d+1]-=1
                k[d]+=10
    else:
        for e in range(len(k)):
            if k[e]>0:
                k[e+1]+=1
                k[e]-=10
        for c in range(len(k)):
            if k[c]<=-10:
                if c!=len(k)-1:
                    k[c+1]-=k[c]//(-10)
                    k[c]%=(-10)
                else:
                    k.append(k[c]//10)
                    k[c]%=10
    return k[::-1]

def sub(x,y):
    if y[0]>0:
        z='-'+list2str(y)
    else:
        z=list2str(y)[1:]
    w=str2list(z)
    return add(x,w)

def mul(x,y):
    if abs(int(list2str(x.copy())))>=abs(int(list2str(y.copy()))):
        i=1
        z=x
        j=int(list2str(y.copy()))
        if j>0:
            while i<j:
                z=add(x,z)
                i+=1
        elif j<0:
            if x[0]>0:
                k=-j
                while i<k:
                    z=add(x,z)
                    i+=1
                z=str2list('-'+list2str(z))
            else:
                z=mul(str2list(list2str(x.copy())[1:]),str2list(list2str(y.copy())[1:]))
        else:
            z=0
        return z
    else:
        return mul(y,x)

def div(x,y):
    w=int(list2str(y.copy()))
    u=[]
    if w>0 and x[0]>0:
        while int(list2str(x))>=w:
            if x[0]!=0:
                z=int(''.join(x[0:len(y)]))
                if z//w>0:
                    u.append(z//w)
                    x[0:len(y)]=str2list(str(z%w))
                else:
                    z=int(''.join(x[0:len(y)+1]))
                    u.append(z//w)
                    x[0:len(y)+1]=str2list(str(z%w))
                v=z%w*10
                while v<w and type(x[-1])==str:
                    u.append(0)
                    v*=10
        return u
    elif w<0 and x[0]>0:
        e=div(x,str2list(list2str(y.copy())[1:]))
        e[-1]+=1
        return str2list('-'+list2str(e))
    elif w>0 and x[0]<0:
        e=div(str2list(list2str(x.copy())[1:]),y)
        e[-1]+=1
        return str2list('-'+list2str(e))
    else:
        return div(str2list(list2str(x.copy())[1:]),str2list(list2str(y.copy())[1:]))

def pow(x,y):
    i=1
    z=x
    j=int(list2str(y.copy()))
    if j>0:
        while i<j:
            z=mul(z,x)
            i+=1
        return z
    elif j==0:
        return [1]
    else:
        print('No requirement to output very small floating point numbers.')
        return [0]

print(list2str(add(str2list('22222222222222'),str2list('8773849905050505'))))
print(8773849905050505+22222222222222)

print(list2str(sub(str2list('11111111'),str2list('9877344555'))))
print(11111111-9877344555)

print(list2str(sub(str2list('345676778778'),str2list('222222'))))
print(345676778778-222222)

print(list2str(mul(str2list('123456'),str2list('789'))))
print(123456*789)

print(list2str(div(str2list('8773849905050505'),str2list('123'))))
print(8773849905050505//123)

print(list2str(pow(str2list('2'),str2list('66'))))
print(2**66)

print(list2str(add(pow(str2list('2'),str2list('100')),str2list(pow(str2list('3'),str2list('50'))))))
print(2**100+3**50)

print(list2str(sub(add(mul(str2list('2'),str2list('100')),mul(str2list('123456'),str2list('789'))),div(str2list('8773849905050505'),str2list('123')))))
print(2*100+123456*789-8773849905050505//123)
