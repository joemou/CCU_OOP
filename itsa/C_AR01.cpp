#include <bits/stdc++.h>
using namespace std;

int main(){
    stack<int> hi;
    int a;
    while(scanf("%d ",&a)!=EOF){
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