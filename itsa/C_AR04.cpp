#include <bits/stdc++.h>
using namespace std;

int main(){
    int num;
    cin>>num;

    for(int k=0;k<num;k++){
        int a,b;
        cin>>a>>b;
        int matrix[a][b];
        for(int i=0;i<a;i++){
            for(int j=0;j<b;j++){
                cin>>matrix[i][j];
            }
        }
        for(int i=0;i<a;i++){
            for(int j=0;j<b;j++){
                if(matrix[i][j]==1){
                    if(matrix[i+1][j]==0||matrix[i-1][j]==0||matrix[i][j+1]==0||matrix[i][j-1]==0){
                        cout<<"0";
                    }
                    else{
                        cout<<"_";
                    }
                }
                else{
                    cout<<"_";
                }
                cout<<" ";
            }
            cout<<endl;
        }
        if(k!=num-1){
            cout<<endl;
        }

    }

}