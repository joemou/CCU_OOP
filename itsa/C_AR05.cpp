#include <bits/stdc++.h>
using namespace std;



int main() {
    int num;
    cin >> num;
    
    int start[99999]={0},end[99999]={0};
    int time[99999]={0};

    for (int k = 0; k < num; k++) {
        cin >> start[k];
        cin >> end[k];
    }

    for(int i=0; i<num; i++)
    {
        for(int j=start[i]; j<end[i]; j++){
            time[j]++;
        }
    }
    int max=0;
    for(int i=0;i<99999;i++)
    {
        if(max<time[i]){
            max=time[i];
        }
    }
    cout<<max<<endl;


    return 0;
}
