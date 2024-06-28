#include <bits/stdc++.h>

using namespace std;



string convertToBase(int number, int base) {
    if (number == 0) return "0";
    
    string result = "";
    char digits[] = "0123456789ABCDEF";
    
    while (number > 0) {
        result += digits[number % base];
        number /= base;
    }
    
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    int base, number;
    
    while (cin >> base >> number) {
        
        string convertedNumber = convertToBase(number, base);
        cout << convertedNumber << endl;
    }
    
    return 0;
}