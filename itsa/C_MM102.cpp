#include <bits/stdc++.h>
using namespace std;



int main() {
    double L, U, D;
    string a,b,c;

    cin>>L>>a;
    cin>>U>>b;
    cin>>D>>c;

    if(a=="m"){
        L*=100;
    }
    if(b=="m"){
        U*=100;
    }
    if(c=="m"){
        D*=100;
    }

    double now=0;
    double day=0;
    while(L!=now){
        day+=1;
        now+=U;
        if(now==L){
            break;
        }
        now-=D;
        if(now<=0){
            break;
        }
    }
    cout<<day<<endl;


}