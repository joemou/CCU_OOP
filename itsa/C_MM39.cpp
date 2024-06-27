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

    a=pow(a,2);
    b=pow(b,2);
    c=pow(c,2);

    if(flag==1&&((a+b)==c||(a+c)==b||(b+c)==a)){
        cout<<"Right Triangle\n";
    }
    else if(flag==1&&((a+b)<c||(a+c)<b||(b+c)<a)){
        cout<<"Obtuse Triangle\n";
    }
    else if(flag==1&&((a+b)>c||(a+c)>b||(b+c)>a)){
        cout<<"Acute Triangle\n";
    } 
    else{
        cout<<"Not Triangle\n";
    }



}