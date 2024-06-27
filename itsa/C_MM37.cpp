#include <bits/stdc++.h>
using namespace std;



int main() {
    double a,b;

    cin>>a>>b;

    if(a>0&&b>0){
        cout<<"1st Quadrant\n";
    }
    if(a<0&&b>0){
        cout<<"2nd Quadrant\n";
    }
    if(a>0&&b<0){
        cout<<"4th Quadrant\n";
    }
    if(a<0&&b<0){
        cout<<"3rd Quadrant\n";
    }
    if(b==0){
        cout<<"x-axis\n";
    }
    if(a==0){
        cout<<"y-axis\n";
    }
    if(a==0&&b==0){
        cout<<"Origin\n";
    }


}