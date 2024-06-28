#include <bits/stdc++.h>
using namespace std;

string decimalToBinary(int num) {
    string binary = "";
    for (int i = 31; i >= 0; i--) {
        int k = num >> i;
        if (k & 1)
            binary += "1";
        else
            binary += "0";
    }
    return binary;
}


int main() {


    int num;
    cin>>num;
    cout<<decimalToBinary(num)<<endl;






}