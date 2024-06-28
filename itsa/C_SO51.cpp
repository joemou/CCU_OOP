#include <bits/stdc++.h>
using namespace std;



int main() {

    vector<int> s;
    int a;
    cin>>a;
    for(int i=0;i<a;i++){
        int num;
        cin>>num;
        s.push_back(num);
    }

    sort(s.begin(),s.end());
    int sum=0,out=0;
    
    for(int i=0;i<a-1;i++){
        sum+=s[i];
        out+=sum;
    }

    cout<<out<<endl;


}