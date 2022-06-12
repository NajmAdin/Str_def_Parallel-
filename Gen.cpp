#include<iostream>
using namespace std;

int main(){
	freopen("quary.txt", "w", stdout);
    //freopen("data.txt", "w", stdout);
    //            100000
    for(int i=0;i<100;i++){
        string s="";
        int x=((rand()%14)+2);
        for(int j=0;j<x;j++){
            unsigned int ch =(rand()*rand());
            s+=(char)((ch%26)+'a');
        }   
        cout<<s<<"\n";
    }
    

    return 0;    
}