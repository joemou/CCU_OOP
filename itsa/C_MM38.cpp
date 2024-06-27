#include <bits/stdc++.h>
using namespace std;



int main() {
    double a, b, c;

    cin>>a>>b>>c;
    int flag=0;

    if((a+b)>c){
        if(c>abs(a-b)){
            flag=1;
        }
    }
    if((a+c)>b){
        if(b>abs(a-c)){
            flag=1;
        }
    }
    if((c+b)>a){
        if(a>abs(c-b)){
            flag=1;
        }
    }
    if(flag){
        cout<<"fit\n";
    }
    else{
        cout<<"unfit\n";
    }


}