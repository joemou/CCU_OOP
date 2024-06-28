#include <bits/stdc++.h>
using namespace std;

int main(){
    double a,b;

    while(cin>>a>>b){
        double c=0;
        if(b==1){
            c=(a-80 )*0.7;
        }
        if(b==2){
            c=(a-70 )*0.6;
        }

        printf("%.1lf\n",c);
    }

}