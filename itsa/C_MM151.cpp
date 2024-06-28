#include <bits/stdc++.h>
using namespace std;



int main() {

    vector<int> s[3];
    float sum;
    int a,b,c,d,e;
    for(int i=0;i<3;i++){


        scanf("%d,%d,%d,%d,%d",&a,&b,&c,&d,&e);
        s[i].push_back(a);
        s[i].push_back(b);
        s[i].push_back(c);
        s[i].push_back(d);
        s[i].push_back(e);

        sort(s[i].begin(),s[i].end());
        sum+=(s[i][2]+s[i][3]+s[i][4]);

    }

    cout<<fixed<<setprecision(2)<<sum/9<<endl;

    

    


}