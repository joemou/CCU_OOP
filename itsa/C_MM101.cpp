#include <bits/stdc++.h>
using namespace std;



int main() {
    int num;
    cin >> num;
    int paper=0;

    paper=num;

    while(paper>=3){
        int newnum=paper/3;
        num+=newnum;
        paper=paper-newnum*3+newnum;
        
    }
    cout<<num<<endl;



    return 0;
}
