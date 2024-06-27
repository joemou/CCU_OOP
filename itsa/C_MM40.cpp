#include <bits/stdc++.h>
using namespace std;



int main() {

    int a;
    cin>>a;
    int sum=0;
    cout<<"1";
    for(int i=1;i<=a;i++){
        if(i!=1){
            cout<<" + "<<i;
        }
        sum+=i;
    }

    cout<<" = "<<sum<<endl;



}