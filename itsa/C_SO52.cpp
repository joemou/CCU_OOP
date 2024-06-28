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

    for(int i=0;i<a;i++){
        cout<<i+1<<" "<<s[a-1-i]<<endl;
    }



}