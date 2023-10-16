import math
import re # 482
import itertools # 438
import string
import minimatrix as mm

'''#(*) 202-Happy Number
a=19#int(input('a='))
def happy(a):
    b=list(str(a))
    a=0
    i=0
    while i<len(b):
        sum=0
        sum+=int(b[i])**2
        a+=sum
        i+=1
    return a
a=happy(a)
i1=0
while i1<1000:
    if a==1:
        print ('happy',end=' ')
        break
    else:
        a=happy(a)
        i1=i1+1
print('end')

# 204-Count Primes
b=100#int(input('b='))
def primes(b):
    primes=list(range(2,b))
    for i2 in primes.copy():
        for n in range(2,i2):
            if i2%n==0:
                primes.remove(i2)
                break
    return primes
print(len(primes(b)))

#(?) 205-Isomorphic Strings 
s1='paper'#input('s1=')
t1='title'#input('t1=')
if len(s1)!=len(t1):
    print(False)
else:
    list1=list(s1)
    list2=list(t1)
    m=1
    n=1
    dict1={}
    dict2={}
    list3=[]
    list4=[]
    for i3 in list1:
        dict1[i3]=m
        m+=1
    for i4 in list2:
        dict2[i4]=n
        n+=1
    for x in dict1:
        list3+=list(str(dict1[x]))
    for y in dict2:
        list4+=list(str(dict2[y]))
    if list3==list4:
        print(True)
    else:
        print(False)

# 217-Contains Duplicate
c=list(str(1231))#list(input('c='))
i5=0
for x in c:
    if c.count(x)>1:
        i5+=1
    else:
        pass
if i5==len(c):
    print(True)
else:
    print(False)

# 219-Contains Duplicate II
nums=list(str(1231))#list(input('nums='))
k=3#int(input('k='))
for i6 in nums.copy():
    while nums.count(i6)>1:
        d=nums.index(i6)
        nums.remove(i6)
        e=nums.index(i6)
        if abs(e+1-d)<=k:
            print(True,end=" ")
            break
    else:
        pass
print('end')

# 231-Power of Two
f=1#int(input('f='))
for i7 in range(0,f):
    if 2**i7==f:
        print('2^({})'.format(i7),end=" ")
        break
    else:
        pass
print('end')

# 242-Valid Anagram
s2='rat'#input('s2=')
t2='cat'#input('t2=')
list5=list(s2)
list6=list(t2)
list5.sort()
list6.sort()
if list5==list6:
    print(True)
else:
    print(False)

# 258-Add Digits
g=38#int(input('g='))
list7=list(str(g))
while len(list7)>1:
    sum1=0
    for i8 in list7:
        sum1+=int(i8)
    list7=list(str(sum1))
else:
    print(list7[0])

# 263-Ugly Number
h=14#int(input('h='))
if h<6:
    pass
else:
    if h%2 and h%3 and h%5 !=0:
        print(False,end=" ")
    else:
        i=primes(h)
        del i[0:3]
        for i9 in i:
            if h%i9==0:
                print(False,end=" ")
                break
print('over')

# 268-Missing Number
j=[9,6,4,2,3,5,7,0,1]#input('A list with distinct nums:j=')
for i10 in range(0,max(j)):
    if j.count(i10)==0:
        print(i10,end=" ")
print()

# 283-Move Zeroes
k=[0,1,0,3,12]#input('A list k=')
for i11 in k:
    if i11==0:
        k.remove(0)
        k.append(0)
print(k)

#(**) 290-Word Pattern
pattren=list('abcb')#list(input('pattern='))
str1='fish cat dog cat'#input('str=')
str2=str1.split()
def form(x):
    o=1
    for i12 in range(0,len(x)):
        if type(x[i12])==int:
            pass
        else:
            l=x[i12]
            while x.count(l)>0:
                x[x.index(l)]=o
        o+=1
    return x
if len(str2)!=len(pattren):
    print(False)
else:
    form(str2)
    form(pattren)
    if str2==pattren:
        print(True)
    else:
        print(False)
    
# 292-Nim Game
l=16#int(input('l='))
if l<=0:
    print('Error')
else:
    if l%4==0:
        print('You lose')
    else:
        print('You win')

# 326-Power of Three
o=27#int(input('o='))
for i13 in range(0,o):
    if 3**i13==o:
        print('3^({})'.format(i13),end=" ")
        break
    else:
        pass
print('end')

# 342-Power of Four
p=1024#int(input('p='))
for i14 in range(0,p):
    if 4**i14==p:
        print('4^({})'.format(i14),end=" ")
        break
    else:
        pass
print('end')

# 344-Reverse String
str3=list("A man, a plan, a canal: Panama")#list(input('str='))
str3.reverse()
print(''.join(str3))

#(*) 345-Reverse Vowels of a String
str4=list("leetcode")#list(input('str='))
str5=[]
str6=str4.copy()
for i15 in str6:
    if i15=='a'or i15=='e'or i15=='i'or i15=='o'or i15=='u':
        str5.append(str6.index(i15))
        str6[str6.index(i15)]=0
    else:
        pass
str7=str5.copy()
str7.reverse()
for i16 in str5:
    str5[str5.index(i16)]=str4[i16]
for i17,i18 in zip(str5,str7):
    str4[i18]=i17
print(''.join(str4))

# 349-Intersection of Two Arrays 
nums1=[1,2,2,1]#input('A list nums1='))
nums2=[2,2]#input('A list nums2='))
def Inters(x,y):
    p=set(x)
    q=set(y)
    list1 = list(p&q)
    for i in list1.copy():
        list1+=[i]*(min(x.count(i),y.count(i))-1)
    return list1
print(Inters(nums1,nums2))

# 350-Intersection of Two Arrays II 
same=[]
for i19 in Inters(nums1,nums2):
    if nums1.count(i19)<=nums2.count(i19):
        same+=[i19]*(nums1.count(i19))
    else:
        same+=[i19]*(nums2.count(i19))
print(same)

# 367-Valid Perfect Square
q=16#int(input('q='))
for i20 in range(1,q+1):
    if q/i20==i20:
        print(True,end=' ')
        break
print('end')

# 383-Ransom Note
str8='aa'#input('str8=')
str9='ba'#input('str9=')
def canConstruct(x,y):
    x=list(x)
    y=list(y)
    for i in x:
        if x.count(i)<=y.count(i):
            print(True,end=' ')
            break
    print('end')
canConstruct(str8,str9)

# 387-First Unique Character in a String
str10="loveleetcode"#input('str=')
def Unique(x):
    list1=list(x)
    for i in list1:
        if list1.count(i)==1:
            return(list1.index(i))
    return -1
print(Unique(str10))

# 389-Find the Difference
str11='abcd'#input('str11=')
str12='bcade'#input('str12=')
list8=list(str11)+list(str12)
for i21 in list8:
    if list8.count(i21)==1:
        print(i21)
        break

# 400-Nth Digit
r=11#int(input('r='))
list9=[]
for i22 in range(1,r+1):
    list9+=list(str(i22))
print(list9[r-1])

#(*) 409-Longest Palindrome
str13='abccccdd'
def LongPal(x):
    list1=list(x)
    list2=list1.copy()
    for i in list1:
        if list2.count(i)%2!=0:
            list2.remove(i)
    for i in list1:
        if list1.count(i)%2!=0:
            list2.append(i)
            break
    return len(list2)
print(LongPal(str13))

# 414-Third Maximum Number
list10=[2,2,3,1]#input('A list=')
list10.sort(reverse=True)
if len(list10)>=3:
    for i22 in list10[0:2]:
        while list10.count(i22)>0:
            list10.remove(i22)
else:
    pass
print(list10[0])

# 434-Number of Segments in a String
str14="Hello, my name is John"#inoput('str=')
list11=str14.split()
print(len(list11))

#(***) 438-Find All Anagrams in a String
s3="abab"#input('s=')
p3="ab"#input('p=')
list12=list(p3)
p5=[]
p6=[]
for i0 in itertools.permutations(list12,len(list12)):
    p4=[]
    for i26 in range(0,len(list12)):
        p4.append(i0[i26])
    p5.append(''.join(p4))
def find(x,y,z=0):
    if x.find(y,z)!=-1:
        p6.append(x.find(y,z))
        z=x.find(y,z)+len(list12)
        find(x,y,z)
    else:
        pass
    return p6
for i27 in p5:
    find(s3,i27)
print(sorted(p6))

# 441-Arranging Coins
n1=8#int(input('n='))
sum2=0
for i23 in range(0,n1):
    if sum2>n1:
        print(i23-2)
        break
    else:
        sum2+=i23

# 443-String Compression
list13=["a","a","b","b","c","c","c"]#input('A list=')
list13.sort()
list14=[]
for i24 in list13:
    if list13.count(i24)>1:
        list14.append(i24)
        list14+=list(str(list13.count(i24)))
        list13[list13.index(i24):list13.count(i24)+list13.index(i24)]=[]
    else:
        list14.append(i24)
print(list14,len(list14))

# 448-Find All Numbers Disappeared in an Array 
list15=[4,3,2,7,8,2,3,1]#input('A list=')
list16=[]
for i25 in range(1,max(list15)):
    if list15.count(i25)==0:
        list16.append(i25)
print(list16)

# 453-Minimum Moves to Equal Array Elements 
list17=[1,2,3]#input('A list=')
step=0
while list17.count(list17[0])!=len(list17):
    list17.sort()
    for i25 in range(0,len(list17)-1):
        list17[i25]=list17[i25]+1
    step+=1
print(step)

# 455-Assign Cookies
list18=[1,2]#input('A list children=')
list19=[1,2,3]#input('A list cookies=')
list18.sort()
list19.sort()
u=0
while len(list18)>len(list19):
    list18.pop()
while len(list18)<len(list19):
    list19.pop(0)
for i26,i27 in zip(list18,list19):
    if i26<=i27:
        u+=1
print(u)

# 459-Repeated Substring Pattern
str15='aba'#input('str=')
for i28 in range(1,len(str15)):
    if str15[0:i28]*(len(str15)//i28)==str15:
        print(True,end=' ')
        break
print('end')

# 461-Hamming Distance
v=bin(1)#bin(int(input('v=')))
w=bin(4)#bin(int(input('w=')))
dist=0
list20=list(v)
list21=list(w)
while len(list20)<len(list21):
    list20.append(0)
while len(list20)>len(list21):
    list21.append(0)
for i29,i30 in zip(list20,list21):
    if i29!=i30:
        dist+=1
print(dist)

#(*) 475-Heaters
list21=[1,2,3,4]#input('A list houses=')
list23=[2]#input('A list heaters=')
list21.sort()
list23.sort()
list22=range(list21[0],list21[-1]+1)
list24=[list22.index(list23[0]),len(list22)-list22.index(list23[-1])-1]
a1=0
for i31 in list23:
    b1=list22.index(i31)
    if (b1-a1)%2==0:
        list24.append((b1-a1)//2)
    else:
        list24.append((b1-a1)//2)
    a1=b1
print(max(list24))

# 476-Number Complement
c1=bin(176)#bin(int(input('c1=')))
list25=list(c1)
for i32 in range(2,len(list25)):
    if list25[i32]=='0':
        list25[i32]='1'
    else:
        list25[i32]='0'
d1=''.join(list25)
print(int(d1,2))

#(*) 479-Largest Palindrome Product
e1=2#int(input('in[1,8]:e1='))
list26=[]
for i33 in range(0,10**e1):
    for i34 in range(0,10**e1):
        list26.append(i33*i34)
for i35 in list26.copy():
    list27=list(str(i35))
    if len(list27)%2==0:
        for i36 in list27:
            if list27.count(i36)%2!=0:
                list26.remove(i35)
                break
    else:
        list27.pop(len(list27)//2)
        for i37 in list27:
            if list27.count(i37)%2!=0:
                list26.remove(i35)
                break
print(max(list26)%1337)

#(**) 482-License Key Formatting
f1="5F3Z-2e-9-w"#input('S=')
K=4#int(input('K='))
list28=f1.split('-')
f1=''.join(list28)
str16=f1[0:len(f1)%K]
f1=f1[::-1]
ins=re.compile('.{%d}'%(K))
g1='-'.join(ins.findall(f1))
g1=g1[::-1]
str16=str.upper(str16)
g1=str.upper(g1)
list29=[str16,g1]
if str16=='':
    print(g1)
else:
    print('-'.join(list29))

#(*) 485-Max Consecutive Ones
list30=[1,1,0,1,1,1]#input('A list=')
list31=[]
i38=0
while i38<len(list30):
    i39=i38
    while i38<len(list30) and list30[i38]==1:
        i38+=1
    else:
        list31.append(i38-i39)
    i38+=1
print(max(list31))

#(*) 496-Next Greater Element I
nums3=[2,4]#input('nums1=')
nums4=[1,2,3,4]#input('nums2=)
list32=[]
for i40 in nums3:
    if nums4[-1]!=i40:
        for i41 in nums4[nums4.index(i40):len(nums4)]:
            if i41>i40:
                list32.append(i41)
                break
            elif i41==i40:
                pass
            else:
                list32.append(-1)
                break
    else:
        list32.append(-1)
print(list32)

# 507-Perfect Number
h1=28#int(input('h1='))
j1=0
for i41 in range(1,h1):
    if h1%i41==0:
        j1+=i41
if j1==h1:
    print(True)
else:
    print(False)

# 628-Maximum Product of Three Numbers
list33=[1,2,3,4]#input('A list=')
list33.sort(reverse=True)
print(list33[0]*list33[2]*list33[1])

# 633-Sum of Square Numbers
l1=8#int(input('l1='))
for i34 in range(1,l1):
    for i35 in range(1,l1):
        if i34**2+i35**2==l1:
            print(True,end=' ')
            l1=0
            break
    if l1==0:
        break
print('end')

###?????????#(***) 672-Bulb Switcher II 
n1=3#int(input('n1='))
m1=2#int(input('m1='))
list34=[1]*n1
list35=[]
def turnall(x):
    for i in range(0,len(x)):
        if x[i]==1:
            x[i]=0
        else:
            x[i]=1
    return x
def turneven(x):
    for i in range(0,len(x),2):
        if x[i]==1:
            x[i]=0
        else:
            x[i]=1
    return x
def turnodd(x):
    for i in range(1,len(x),2):
        if x[i]==1:
            x[i]=0
        else:
            x[i]=1
    return x
def turn3k1(x):
    for i in range(1,len(x),3):
        if x[i]==1:
            x[i]=0
        else:
            x[i]=1
    return x
if m==1:
    list35=[turnall(list34.copy()),turnodd(list34.copy()),turneven(list34.copy()),turn3k1(list34.copy())]
elif m==2:
    pass

# 728-Self Dividing Numbers 
left=1#int(input('left='))
right=22#int(input('right='))
list38=list(range(left,right+1))
for i36 in list38.copy():
    list37=list(str(i36))
    for i37 in list37:
        if  i37=='0' or i36%int(i37)!=0:
            list38.remove(i36)
            break
print(list38)

###?????????#(**) 754-Reach a Number 
target=3#int(input('target='))


# 877-Stone Game
piles=[5,3,4,5]#input('piles list=')
if piles==[] or len(piles)%2!=0 or sum(piles)%2==0:
    print('Wrong')
else:
    list39=[]
    list40=[]
    while len(piles)>0:
        i39=max(piles[0],piles[-1])
        list39.append(i39)
        piles.remove(i39)
        i39=max(piles[0],piles[-1])
        list40.append(i39)
        piles.remove(i39)
    if sum(list39)>sum(list40):
        print(True)
    else:
        print(False)

# 914-X of a Kind in a Deck of Cards
deck=[1,1,1,2,2,2,3,3]#input('deck list=')
for i40 in deck:
    if deck.count(i40)==1:
        print(False,end=' ')
for X in range(2,len(deck)+1):
    for i40 in deck:
        if deck.count(i40)%X!=0:
            break
        else:
            print(True,end=' ')
            break
print('end')

# 942-DI String Match
S=list('DDI')#list(input('I or D string:'))
A=list(range(0,len(S)+1))
def DI(S,A):
    for i41 in range(0,len(S)):
        if S[i41]=='D':
            if A[i41]<A[i41+1]:
                o1=A[i41]
                A[i41]=A[i41+1]
                A[i41+1]=o1
                DI(S,A)
    return A
print(DI(S,A))

# 7-Reverse Integer
p1=list(str(-1230))#list(input('p1='))
p1.reverse()
while p1[0]=='0':
    p1.pop(0)
if p1[-1]=='-':
    p1[:0]=p1[-1]
    p1.pop()
print(int(''.join(p1)))

# 9-Palindrome Number
q1=list(str(-121))#list(input('q1='))
if q1==q1[::-1]:
    print(True)
else:
    print(False)

# 11-Container With Most Water
r1=[1,8,6,2,5,4,8,3,7]#input('A list=')
list41=[]
for i42 in r1:
    for i43 in r1:
        if i42<=i43:
            list41.append(i42*abs(r1.index(i42)-r1.index(i43)))
print(max(list41))

# 12-Integer to Roman
u1=1994#int(input('u1='))
v1=u1//1000
w1=(u1-1000*v1)//100
x1=(u1-1000*v1-100*w1)//10
y1=u1-1000*v1-100*w1-10*x1
list42=['M']*v1
if w1==4:
    list42.append('CD')
elif w1==9:
    list42.append('CM')
elif w1<4:
    list42.append("C"*w1)
else:
    list42.append('D'+'C'*(w1-5))
if x1==4:
    list42.append('XL')
elif x1==9:
    list42.append('XC')
elif x1<4:
    list42.append("X"*x1)
else:
    list42.append('L'+'X'*(x1-5))
if y1==4:
    list42.append('IV')
elif y1==9:
    list42.append('IX')
elif y1<4:
    list42.append("I"*y1)
else:
    list42.append('V'+'I'*(y1-5))
print(''.join(list42))

# 14-Longest Common Prefix
a=["flower","flow","flight"]#input('A list with string:')
b=list(a[0])
for i in range(1,len(a)):
    while len(b)>len(list(a[i])):
        b.pop()
    for j,k in zip(list(a[i]),b.copy()):
        if j!=k:
            b.remove(k)
print('"{}"'.format(''.join(b)))

# 26-Remove Duplicates from Sorted Array
a=[0,0,1,1,1,2,2,3,3,4]#input('A list sorted:')
for i in a.copy():
    while a.count(i)>1:
        a.remove(i)
print(len(a))

# 27-Remove Element
a=[0,1,2,2,3,0,4,2]#input('A list:')
b=2#int(input('val='))
while a.count(b)>0:
    a.remove(b)
print(len(a))

# 28-Implement strStr()
a="aaaaa"#input('haystack=')
b='bba'#input('needle=')
print(a.find(b))

# 34-Find First and Last Position of Element in Sorted Array
a=[5,7,7,8,9,10]#input('A list nums:')
b=6#int(input('target='))
c=[]
if a.count(b)>0:
    c.append(a.index(b))
    a.reverse()
    c.append(len(a)-1-a.index(b))
else:
    c=[-1,-1]
print(c)

# 35-Search Insert Position
a=[1,3,5,6]#input('A list sorted:')
b=7#int(input('target='))
if a.count(b)>0:
    print(a.index(b))
else:
    a.append(b)
    a.sort()
    print(a.index(b))

# 50-Pow(x, n)
a=2.1#float(input('x='))
b=-3#int(input('n='))
print('{:.5f}'.format(a**b))

# 58-Length of Last Word
a="Hello World"#input('a=')
b=a.split()
print(len(b[-1]))

###?????????# 63-Unique Paths II 

###?????????# 64-Minimum Path Sum 

# 66-Plus One
a=[0,2,3,9]#input('A list:')
b=[]
while a!=[0] and a[0]==0:
    a.pop(0)
for i in a:
    b.append(str(i))
c=list(str(int(''.join(b))+1))
b=[]
for j in c:
    b.append(int(j))
print(b)

# 67-Add Binary
a='1010'#input('a=')
b='1011'#input('b=')
c=int('0b'+a,2)
d=int('0b'+b,2)
e=str(bin(c+d))
print(e[2:len(e)])

# 69-Sqrt(x)
a=8#int(input('a='))
print(int(a**(0.5)))

# 70-Climbing Stairs
a=3#int(input('a='))
b=a//2
d=0
for i in range(1,b+1):
    c=a-i
    d+=math.factorial(c)//(math.factorial(i)*math.factorial(c-i))
print(d+1)

# 88-Merge Sorted Array
nums1=[1,2,3,0,0,0]
nums2=[2,5,6]
def del0(x):
    while x.count(0)>0:
        x.remove(0)
    return x
nums1=del0(nums1)
nums2=del0(nums2)
print(sorted(nums1+nums2))

# 121-Best Time to Buy and Sell Stock
a=[7,6,4,3,1]
b=min(a)
c=[]
for i in range(a.index(b),len(a)):
    if a[i]>=b:
        c.append(a[i]-b)
print(max(c))

# 125-Valid Palindrome
a="A man, a plan, a canal: Panama"
b=list(a.lower())
for i in b.copy():
    if i not in string.ascii_letters:
        b.remove(i)
if b==b[::-1]:
    print(True)
else:
    print(False)

# 136-Single Number, 137-Single Number II
def single(x):
    for i in x:
        if x.count(i)==1:
            return i
a=[0,1,0,1,0,1,99]
print(single(a))

# 167-Two Sum II - Input array is sorted
numbers=[2,7,11,15,16]
target=18
k=[]
for i in numbers:
    for j in numbers[numbers.index(i):]:
        if j==target-i:
            k=[numbers.index(i)+1,numbers.index(j)+1]
            break
    if k!=[]:
        break
print(k)

# 169-Majority Element
list=[2,2,1,1,1,2,2]
for i in list:
    if list.count(i)>len(list)//2:
        print(i)
        break 

# 172-Factorial Trailing Zeroes
a=5
b=math.factorial(a)
c=list(str(b))[::-1]
for i in c:
    if i!='0':
        print(c.index(i))
        break

# 189-Rotate Array
list=[-1,-100,3,99]
k=2
for i in range(k):
    n=list.pop()
    list.reverse()
    list.append(n)
    list.reverse()
print(list)

# 190-Reverse Bits
a=43261596
b=list(bin(a))[2:]
b.reverse()
if len(b)<32:
    b+=['0']*(32-len(b))
print(int(''.join(b),2))

# 191-Number of 1 Bits 
a=127
b=list(bin(a))
print(b.count('1'))

# 746-Min Cost Climbing Stairs
cost=[1, 100, 1, 1, 1, 100, 1, 1, 100, 1]
if len(cost)>3:
    if cost[0]>=cost[1]:
        i=1
    else:
        i=0
    sum=0
    while i<len(cost):
        sum+=cost[i]
        if i!=len(cost)-1 and i!=len(cost)-2:
            if cost[i+1]>=cost[i+2]:
                i+=2
            else:
                i+=1
        else:
            i+=2
elif len(cost)==3:
    if cost[1]<=cost[0]+cost[2]:
        sum=cost[1]
    else:
        sum=cost[0]+cost[2]
else:
    sum=min(cost)
print(sum)

# 867-Transpose Matrix
a=mm.Matrix([2,3],[[1,2,3],[4,5,6]])
print(a.T().data)

# 868-Binary Gap
a=list(str(bin(22)))
list1=[0]*a.count('1')
for i in range(len(list1)):
    list1[i]=a.index('1')
    a[a.index('1')]=0
b=0
for i in range(len(list1)-1):
    if list1[i+1]-list1[i]>b:
        b=list1[i+1]-list1[i]
print(b)

# 884-Uncommon Words from Two Sentences
a = "this apple is sweet"
b = "this apple is sour" 
c = a.split(' ')
d = b.split(' ')
e = [i for i in c+d if (c+d).count(i)==1]
print(e)

# 888-Fair Candy Swap
a = [1,2,5] 
b = [2,4]
c = [0,0]
for i in range(len(a)):
    for j in range(len(b)):
        x,y=a[i],b[j]
        a[i],b[j]=y,x
        if sum(a)==sum(b):
            c[0],c[1]=x,y
            break
        a[i],b[j]=x,y
    if c!=[0,0]:
        break
print(c)

#????????? 893-Groups of Special-Equivalent Strings

# 896-Monotonic Array
a = [1,3,2]
b,c = sorted(a),sorted(a,reverse=True)
if a==b or a==c:
    print(True)
else:
    print(False)
   
# 905-Sort Array By Parity 
a=[3,1,2,4]
print([x for x in a if x%2==0]+[j for j in a if j%2==1])

# 908-Smallest Range I
a,k=[0,10],3
b=sorted(a)
if b[-1]-(b[0]+b[-1])/2<=k:
    print(0)
else:
    print(b[-1]-b[0]-k-k)
    
# 917-Reverse Only Letters
a="Test1ng-Leet=code-Q!"
b=list(a)
c,d=[],[]
for i in b:
    if i not in string.ascii_letters:
        c.append(b.index(i)+len(c))
        d.append(i)
        b.remove(i)
b.reverse()
for j,k in zip(c,d):
    b.insert(j,k)
print(''.join(b))

# 921-Minimum Add to Make Parentheses Valid 
a='()))(('
b=list(a)
sum=0
for i in range(len(b)):
    if b[i]=='(':
        if b[i:].count(')')>0:
            b[i]=0
            b[b[i:].index(')')+i]=0
        else:
            sum+=1
    elif b[i]==')':
        sum+=1
print(sum)

# 922-Sort Array By Parity II
a=[4,2,3,0,9,5,6,8,7,4,2,1,7,3,6,1]
b=[-1]*len(a)
for i in a:
    if i%2 == 0:
        for n in range(0,len(b)-1,2):
            if b[n] == -1:
                b[n] = i
                break
    else:
        for n in range(1,len(b),2):
            if b[n] == -1:
                b[n] = i
                break
print(b)

# 925-Long Pressed Name
name,typed='aseed','ssaadd'
dict1={i:name.count(i) for i in name}
dict2={i:typed.count(i) for i in typed}
s=0
for j in dict1:
    if j not in dict2 or dict1[j]>dict2[j]:
        s=1
        break
print(False if s==1 else True)
'''
#??????? 932-Beautiful Array 


