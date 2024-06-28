#include <bits/stdc++.h>

using namespace std;

int f91(int n)
{
    if(n<=100){
        return f91(f91(n+11));
    }
    else if(n>=101){
        return n-10;
    }

    return 0;
}

int main()
{
    int a,b;
    cin >> a;
    while(a--)
    {
        cin >>b;
        cout << f91(b) << endl;
    }

    return 0;
}