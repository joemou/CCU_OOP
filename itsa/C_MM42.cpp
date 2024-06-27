#include <bits/stdc++.h>
using namespace std;



int main() {

    int n;
    double Sum=0;
    cin >> n;

    for(int i=1; i<=n; i++){
        Sum+=pow(-1.0,(i+1)*1.0)*1/(2*i-1);
    }

    cout << fixed << setprecision(3) << Sum << endl;

    return 0;

}