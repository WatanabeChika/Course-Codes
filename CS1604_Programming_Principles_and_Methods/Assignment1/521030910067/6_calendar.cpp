#include <iostream>
#include <vector>

using namespace std;

// This program would find all the days in month a and is the bth (week-number)c in the period between y1 and y2.
int main(){
    int a,b,c,y1,y2;
    cin>>a>>b>>c>>y1>>y2;
    int leap=0, day=2;
    for (int i=1850; i!=y1; ++i){
        if ((i%4==0 && i%100!=0) || i%400==0)
            leap+=1;
    };
    day=(2+366*leap+365*(y1-1850-leap))%7;
    for (int j=y1; j!=y2+1; ++j){
        int Feb=28;
        if ((j%4==0 && j%100!=0) || j%400==0) Feb=29;
        vector<int> months={31,Feb,31,30,31,30,31,31,30,31,30,31};
        for (int k=0; k!=a-1; ++k)
            day+=months[k];
        day%=7;
        int date=1;
        c%=7;
        if (day<c)
            date+=c-day;
        else if(day>c)
            date+=c-day+7;
        date+=7*(b-1);
        if (date>months[a-1])
            cout<<"none"<<endl;
        else{
            if (a>=10){
                if (date>=10) cout<<j<<"/"<<a<<"/"<<date<<endl;
                else cout<<j<<"/"<<a<<"/0"<<date<<endl;
            } else{
                if (date>=10) cout<<j<<"/0"<<a<<"/"<<date<<endl;
                else cout<<j<<"/0"<<a<<"/0"<<date<<endl;
            };
        };
        for (int m=a-1; m!=12; ++m)
            day+=months[m];
        day%=7;
    };
    return 0;
}