#include <bits/stdc++.h>
using namespace std;



int main() {

    int num;
    vector<int>b;

    cin>>num;

    while(num--){
        int a;
        cin>>a;
        b.push_back(a);

    }
    for(auto it:b){
        int flag=1;

        for(int i=2; i<it ;i++){
            if(it%i==0){
                flag=0;
            }


        }
        if(flag){
            cout<<it<<endl;
        }
    }



}