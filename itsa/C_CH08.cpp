#include <bits/stdc++.h>
using namespace std;

int main(){
    int a,b,c,d,e;

    cin>>a>>b>>c>>d>>e;

    int sum=(a*1+b*2+c*2+d*2)-(e*2);

    if(sum>=45){
        cout<<"A"<<endl;
    }
    else if(sum>=35&&sum<=44){
        cout<<"B"<<endl;
    }
    else if(sum>=25&&sum<=34){
        cout<<"C"<<endl;
    }
    else if(sum<=24){
        cout<<"D"<<endl;
    }
}