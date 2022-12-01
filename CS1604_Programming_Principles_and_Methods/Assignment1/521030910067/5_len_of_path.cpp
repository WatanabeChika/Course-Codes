#include <iostream>

using namespace std;

// This program would calculate the lengths of the two walking to the point in a clockwise path.
int main(){
    long long n,i,j,pa1=0,pa2=0;
    cin>>n>>i>>j;
    while (i!=1 && j!=1 && i!=n && j!=n){
        pa1+=4*(n-1);
        pa2+=4*(n-1);
        --i;--j;
        n-=2;
    };
    if (j==1){
        pa1+=n-i;
        pa2+=2*(n-1)+n-i;
    } else if(i==1 && j!=1 && j!=n){
        pa1+=n-1+j-1;
        pa2+=3*(n-1)+j-1;
    } else if(j==n){
        pa1+=2*(n-1)+i-1;
        pa2+=i-1;
    } else if(i==n && j!=1 && j!=n){
        pa1+=3*(n-1)+n-j;
        pa2+=n-1+n-j;
    };
    cout<<pa1<<" "<<pa2<<endl;
    return 0;
}