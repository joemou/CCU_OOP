#include <bits/stdc++.h>
using namespace std;

int main(){
    stack<int> hi;
    int a;
    while(hi.size()!=6){
        cin>>a;
        hi.push(a);
    }
    cout<<hi.top();
    hi.pop();
    while(!hi.empty()){
        cout<<" "<<hi.top();
        hi.pop();
    }
    cout<<endl;
}