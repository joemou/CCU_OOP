#include <bits/stdc++.h>
using namespace std;

int main(){
    stack<int> hi;
    int a;
    while(hi.size()!=6){
        cin>>a;
        hi.push(a);
    }
    int sum=0;
    while(!hi.empty()){
        sum+=pow(hi.top(),3);
        hi.pop();
    }
    cout<<sum<<endl;
}