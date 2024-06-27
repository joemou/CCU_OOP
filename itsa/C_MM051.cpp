#include <bits/stdc++.h>
using namespace std;


int main(){
    queue<int> ans;
    int num;

    cin>>num;
    int count=0;
    int sum=0;
    for(int i=2;i<=999999;i++){
        for(int j=2;j<=i;j++){
            if(i==j){
                count++;
                ans.push(j);
                sum+=j;
            }
            else if(i%j==0){
                break;
            }
        }
        if(count==num){
            break;
        }

    }
    while(!ans.empty()){
        cout<<ans.front()<<",";
        ans.pop();
    }
    cout<<endl<<sum<<endl;
}