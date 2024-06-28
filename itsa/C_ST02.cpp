#include <bits/stdc++.h>

using namespace std;





int main() {
    int a;

    cin>>a;
    
    string b;

    while (a--) {
        cin>>b;
        reverse(b.begin(),b.end());
        cout<<b<<endl;
    }
    
    return 0;
}