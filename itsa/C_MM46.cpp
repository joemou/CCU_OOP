#include <bits/stdc++.h>
using namespace std;



int main() {

    int num;

    cin>>num;

    char op;
    double a,b,c,d;
    double x,y;

    for(int i=0;i<num;i++){
        cin>>op>>a>>b>>c>>d;
        if(op=='+'){
            x=a+c;
            y=b+d;
        }
        else if(op=='-'){
            x=a-c;
            y=b-d;
        }
        else if(op=='*'){
            x=(a * c) - (b * d);
            y=(b * c) + (a * d);
        }
        else if(op=='/'){
            x=((a * c) + (b * d)) / (c * c + d * d);
            y=((b * c) - (a * d)) / (c * c + d * d);
            
        }
        cout<<x<<" "<<y<<endl;
    }

    

}