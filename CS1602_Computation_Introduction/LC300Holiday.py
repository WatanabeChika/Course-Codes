from cgitb import reset
import math

# 1.Happy Number
def is_happy_number(n):
    setty=set()
    while n not in setty:
        setty.add(n)
        lis=list(str(n))
        summ=0
        for i in lis:
            summ+=int(i)
        n=summ
    return True if n==1 else False

# 2.Count Primes
def primes_below_it(x):
    primes=[1]*(x-1)
    for i in range(2,int(x**(0.5))+1):
        if primes[i-2]!=-1:
            for j in range(i,x//i+1):
                primes[i*j-2]=-1
    return len(primes)-primes.count(-1)

# 3.Isomorphic Strings
def are_isomorphic(str1,str2):
    if len(str1)!=len(str2):
        return False
    else:
        dic,setty={},set()
        for i in range(len(str1)):
            if str1[i] in dic:
                if str2[i]!=dic[str1[i]]:
                    return False
            elif str2[i] in setty:
                return False
            else:
                setty.add(str2[i])
                dic[str1[i]]=str2[i]
        return True

# 4.Contains Duplicate
def duplicated_I(lis):
    for i in lis:
        if lis.count(i)>1:
            return True
    return False

# 5.Contains Duplicate II
def duplicated_II(lis,k):
    for i in lis.copy():
        if lis.count(i)>1:
            ind1=lis.index(i)
            lis[ind1]=i+1
            ind2=lis.index(i)
            if ind2-ind1<=k:
                return True
            else:
                lis[ind1]=i
    return False

# 6.Power of Two / 14.Power of Three / 15.Power of Four
def is_power_of_num(n,m):
    for i in range(n//2+1):
        if m**i==n:
            return True
    return False

# 7.Valid Anagram
def are_anagram(str1,str2):
    lis1,lis2=sorted(list(str1)),sorted(list(str2))
    return True if lis1==lis2 else False

# 8.Add Digits
def digit_add(n):
    if n<0:
        return 'Wrong'
    elif 0<=n<10:
        return n
    else:
        lis=list(str(n))
        return digit_add(sum(int(i) for i in lis))

# 9.Ugly Number
def is_ugly(n):
    if n<1:
        return False
    else:
        for i in [2,3,5]:
            while n%i==0:
                n//=i
        return n==1

# 10.Missing Number
def missing_num(lis):
    for i in range(max(lis)):
        if lis.count(i)==0:
            return i

# 11.Move Zeroes
def move_zero(lis):
    for i in lis.copy():
        if i==0:
            lis.remove(i)
            lis+=[0]
    return lis

# 12.Word Pattern
def word_pattern(patt,word):
    lis1,lis2=list(patt),word.split()
    dic={}
    for i,j in zip(lis1,lis2):
        if i not in dic:
            dic[i]=j
        else:
            if dic[i]!=j:
                return False
    return True

# 13.Nim Game
def nim_game(n):
    if n<=0:
        return 'Wrong'
    return 'Lose' if n%4==0 else 'Win'

# 17.Reverse Vowels of a String
def rev_vowels(strr):
    lis=list(strr)
    vos=[]
    for i in lis:
        if i in ['a','e','o','i','u']:
            lis[lis.index(i)]=0
            vos+=[i]
    for j in lis:
        if j==0:
            lis[lis.index(j)]=vos.pop()
    return ''.join(lis)

# 18.Intersection of Two Arrays
def intsec_of_arraysI(lis1,lis2):
    setty1,setty2=set(lis1),set(lis2)
    return list(setty1&setty2)

# 19.Intersection of Two Arrays II
def intsec_of_arraysII(lis1,lis2):
    lis=intsec_of_arraysI(lis1,lis2)
    for i in lis.copy():
        lis+=[i]*(min(lis1.count(i),lis2.count(i))-1)
    return lis

# 20.Valid Perfect Square
def is_squarable(n):
    if n<0:
        return False
    else:
        for i in range(n+1):
            if n//i==i:
                return True
        return False

# 21.Ransom Note
def can_construct(str1,str2):
    x,y=list(str1),list(str2)
    return x==intsec_of_arraysII(x,y)

# 22.First Unique Character in a String
def unique_char_1st(strr):
    for i in strr:
        if strr.count(i)==1:
            return strr.index(i)
    return -1

# 23.Find the Difference
def find_diff(init_str,oper_str):
    str1,str2=sorted(init_str),sorted(oper_str)
    for i,j in zip(str1,str2):
        if i!=j:
            return j
    return

# 24.Nth Digit
def n_digit(n):
    lis,res=[i for i in range(1,n+1)],[]
    for i in lis:
        res+=list(str(i))
        if len(res)>=n:
            return int(res[n-1])

# 25.Longest Palindrome
def long_palin(strr):
    list1=list(strr)
    summ=0
    for i in list1:
        if list1.count(i)%2!=0:
            summ+=1
    return len(list1)-summ+1

# 26.Third Maximum Number
def find_3rd_max(lis):
    lis.sort(reverse=True)
    dic={lis[i]:i for i in range(len(lis))}
    if len(dic)<3:
        return min(dic)
    else:
        return list(dic.keys())[2]

# 27.Find All Anagrams in a String
def all_anas(strr,examp):
    lis=[]
    for i in range(len(strr)-len(examp)+1):
        stri=strr[i:i+len(examp)]
        if are_anagram(stri,examp):
            lis+=[i]
    return lis

# 28.Arranging Coins
def arr_coin(n):
    summ=0
    for i in range(n+1):
        summ+=i
        if summ>=n:
            return i

# 29.String Compression
def str_comp(lis):
    relis=[]
    for i in set(lis):
        if lis.count(i)>1:
            relis+=[i]+list(str(lis.count(i)))
        else:
            relis+=[i]
    return relis

# 30.Find All Numbers Disappeared in an Array
def find_disap_num(lis):
    rel=[]
    for i in range(1,max(lis)):
        if lis.count(i)==0:
            rel+=[i]
    return rel

# 31.Minimum Moves to Equal Array Elements
def min_move_to_equal(lis):
    step=0
    while lis.count(lis[0])!=len(lis):
        lis.sort()
        lis[-1]-=1
        step+=1
    return step

# 32.Assign Cookies
def assign_cookies(kids,cookies): # type=list
    kids,cookies=sorted(kids),sorted(cookies)
    ans=0
    for i in cookies:
        if ans<len(kids) and i>=kids[ans]:
            ans+=1
    return ans

# 33.Repeated Substring Pattern
def rep_sub_patt(strr):
    for i28 in range(1,len(strr)):
        if strr[:i28]*(len(strr)//i28)==strr:
            return True
    return False

# 34.Hamming Distance
def ham_dis(x,y):
    ans=0
    m,n=bin(x)[2:],bin(y)[2:]
    if len(m)>len(n):
        n='0'*(len(m)-len(n))+n
    else:
        m='0'*(len(n)-len(m))+m
    for i,j in zip(m,n):
        if i!=j:
            ans+=1
    return ans

# 35.Heaters
def heaters(houses,heaters):
    houses,heaters=sorted(houses),sorted(heaters)
    ans=0
    heat=heaters.copy()
    while len(heat)<len(houses) and houses != heat[:len(houses)]:
        ans+=1
        for i in heaters:
            heat+=[i+ans,i-ans]
        heat.sort()
    return ans

# 36.Number Complement
def num_compl(n):
    lis=list(bin(n))
    for i in range(2,len(lis)):
        if lis[i]=='0':
            lis[i]='1'
        else:
            lis[i]='0'
    return int(''.join(lis),2)

# 37.Largest Palindrome Product (待思考)
def large_pal_prod(n):
    if n==1:
        return 9
    else:
        for a in range(2,10**n):
            left=10**n-a
            right=int(str(left)[::-1])
            if a**2-4*right>=0:
                x=a-(a**2-4*right)**0.5
                if x//2==x/2:
                    return (10**n*left+right)%1337

# 38.License Key Formatting
def lic_key_form(lic,k):
    f1=lic.replace('-','')[::-1]
    f1=str.upper(f1)
    f2=''
    for i in range(0,len(f1),k):
        f2+=f1[i:i+k]
        if i+k<len(f1):
            f2+='-'
    return f2[::-1]

# 39.Max Consecutive Ones
def max_con_one(lis):
    ans,length=[],0
    for i in range(len(lis)):
        if lis[i]==1:
            length+=1
        else:
            ans+=[length]
            length=0
    ans+=[length]
    return max(ans)

# 40.Next Greater Element I
def next_greater_ele(nums1,nums2):
    lis=[]
    for i in nums1:
        for j in nums2[nums2.index(i):]:
            z=-1
            if j>i:
                z=j
                break
        lis+=[z]
    return lis

# 41.Perfect Number
def perfect_num(n):
    lis=[1]
    for i in range(2,int(n**0.5)+1):
        if n%i==0:
            lis+=[i,n//i]
    setty=set(lis)
    return True if n==sum(setty) else False

# 42.Sum of Square Numbers
def is_sum_of_squ(n):
    i,j=1,0
    while j<n:
        j+=i**2
        if j==n:
            return True
        i+=1
    return False

# 43.Bulb Switcher II (待思考)
def bulb_switcher(n,m):
    if m==0 or n==0:
        return 1
    if n==1:
        return 2
    if n==2:
        if m==1:
            return 3
        return 4
    if n>=3:
        if m==1:
            return 4
        if m==2:
            return 7
        return 8

# 44.Self Dividing Numbers
def is_self_div_num(n):
    lis=list(str(n))
    for i in lis:
        if n%int(i)!=0:
            return False
    return True

# 45.Reach a Number (待思考)
def reach_a_number(target):
    target=abs(target)
    x=math.ceil((2*target+0.25)**0.5-0.5)
    return x if (x*(x+1)/2-target)%2==0 else x+1+x%2

# 46.Stone Game
def stone_game(pile): # type=list
    if pile==[] or sum(pile)%2==0 or len(pile)%2!=0:
        return 'Wrong'
    else:
        Alex,Lee=[],[]
        while len(pile)>0:
            if pile[0]>=pile[-1]:
                Alex+=[pile[0]]
                pile.pop(0)
            else:
                Alex+=[pile[-1]]
                pile.pop()
            if pile[0]>=pile[-1]:
                Lee+=[pile[0]]
                pile.pop(0)
            else:
                Lee+=[pile[-1]]
                pile.pop()
        return True if sum(Alex)>sum(Lee) else False

# 47.X of a Kind in a Deck of Cards
def X_decks_cards(cards): # type=list
    def cal_div(lst,y):
        for i in lst:
            if i%y!=0:
                return False
        return True
    lis=[]
    for j in set(cards):
        lis+=[cards.count(j)]
    for x in range(2,max(lis)+1):
        if cal_div(lis,x):
            return True
    return False

# 48.DI String Match
def DI_str_match(strr):
    def DI(S,A):
        for i in range(0,len(S)):
            if S[i]=='D':
                if A[i]<A[i+1]:
                    c=A[i]
                    A[i]=A[i+1]
                    A[i+1]=c
                    DI(S,A)
        return A
    return DI(list(strr),range(len(strr)+1))

# 49.Reverse Integer
def reverse_int(n):
    p1=list(str(n))
    while p1[0]=='0':
        p1.pop(0)
    if p1[-1]=='-':
        p1[:0]=p1[-1]
        p1.pop()
    return int(''.join(p1))

# 50.Container With Most Water
def cont_most_water(lis):
    ans=[]
    for i in lis:
        for j in lis:
            if i<=j:
                ans.append(i*abs(lis.index(i)-lis.index(j)))
    return max(ans)

# 51.Integer to Roman
def int_to_roman(num): # 0<=num<=3999
    unit = ['','I','II','III','IV','V','VI','VII','VIII','IX']
    ten = ['','X','XX','XXX','XL','L','LX','LXX','LXXX','XC'] 
    hundred = ['','C','CC','CCC','CD','D','DC','DCC','DCCC','CM'] 
    thousand = ['','M','MM','MMM']
    d = [unit,ten,hundred,thousand]
    num = str(num)[::-1]
    ans = []
    for i in range(len(num)):
        ans.append(d[i][int(num[i])])
    return "".join(ans[::-1])

# 52.Longest Common Prefix
def longest_common_prefix(lis): # lis[string]
    x=list(lis[0])
    for i in range(1,len(lis)):
        while len(x)>len(list(lis[i])):
            x.pop()
        for j,k in zip(list(lis[i]),x.copy()):
            if j!=k:
                x.remove(k)
    return '"{}"'.format(''.join(x))

# 53.Find First and Last Position of Element in Sorted Array
def start_end_of(lis,n):
    c=[]
    if lis.count(n)>0:
        c.append(lis.index(n))
        lis.reverse()
        c.append(len(lis)-1-lis.index(n))
    else:
        c=[-1,-1]
    return c

# 54.Search Insert Position
def search_insert(lis,n):
    if n in lis:
        return lis.index(n)
    else:
        lis+=[n]
        return sorted(lis).index(n)

# 55.Unique Paths II (待思考)
def unique_path(lis):
    '''def paths(lis):
        if lis==[] or lis[0]==[]:
            return []
        else:
            lis2=[]
            for i in lis:
                lis2+=[i[:-1]]
            return [unique_path(lis2)+[lis[-1][-1]],unique_path(lis[:-1])+[lis[-1][-1]]]
    return paths(lis)
print(unique_path([[0,0,0],[0,1,0],[0,0,0]]))'''
    pass

# 56.Minimum Path Sum（待思考）
def min_path_sum(lis):
    pass

# 57.Plus One
def plus_one(lis):
    n=int(''.join(lis))
    return list(str(n+1))

# 58.Climbing Stairs
def climb_stairs(n):
    if n==1:
        return 1
    elif n==2:
        return 2
    else:
        return climb_stairs(n-1)+climb_stairs(n-2)

# 59.Best Time to Buy and Sell Stock
def best_to_buy_sell(lis):
    ans=[0]
    for i in range(len(lis)):
        for j in lis[i:]:
            if i<j:
                ans+=[j-i]
    return max(ans)

# 60.Two Sum II
def two_sum(nums,target):
    dic={i:target-i for i in nums}
    for j in dic:
        if dic[j] in dic:
            return [nums.index(j)+1,nums.index(dic[j])+1]

# 61.Factorial Trailing Zeroes
def fac_tra_zeros(n):
    lis=list(str(math.factorial(n)))[::-1]
    for i in lis:
        if i!=0:
            return lis.index(i)

# 62.Min Cost Climbing Stairs
def min_cost_climb_stairs(cost):
    if len(cost)>3:
        i=1 if cost[0]>=cost[1] else 0
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
        return sum
    elif len(cost)==3:
            return cost[1] if cost[1]<=cost[0]+cost[2] else cost[0]+cost[2]
    else:
        return min(cost)

# 63.Binary Gap
def binary_gap(n):
    lis=[0]*list(str(bin(n))).count('1')
    for i in range(len(lis)):
        lis[i]=n.index('1')
        n[n.index('1')]=0
    b=0
    for i in range(len(lis)-1):
        if lis[i+1]-lis[i]>b:
            b=lis[i+1]-lis[i]
    return b

# 64.Fair Candy Swap
def fair_candy_swap(Alice,Bob):
    dis=sum(Alice)-sum(Bob)
    dic={i:i-dis//2 for i in Alice}
    for j in dic:
        if dic[j] in Bob and j!=dic[j]:
            return [j,dic[j]]

# 65.Groups of Special-Equivalent Strings（待思考）
def groups_spec_equ_str(lis): # list[strings]
    pass

# 66.Smallest Range I
def smallest_range(lis,k):
    b=sorted(lis)
    return 0 if b[-1]-(b[0]+b[-1])/2<=k else b[-1]-b[0]-2*k

# 67.Reverse Only Letters
def rev_letters(strr):
    pass
    