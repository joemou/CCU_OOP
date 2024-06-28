#include <bits/stdc++.h>
using namespace std;

vector<int> c(int start) {
    vector<int> s;
    while (start != 1) {
        s.push_back(start);
        if (start % 2 == 0) {
            start /= 2;
        } else {
            start = 3 * start + 1;
        }
    }
    s.push_back(1);
    return s;
}

int main() {
    int start;

    cin >> start;



    vector<int> sequence = c(start);
    for (int num : sequence) {
        cout << num ;
        if(num!=1){
            cout<< "\n";
        }
        
    }
    cout << std::endl;

    return 0;
}
