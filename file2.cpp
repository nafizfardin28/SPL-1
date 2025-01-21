#include<iostream>
using namespace std;
void sayHello(){
    cout << "Hello" << endl;
}
int addNumber(){
    return 5+5;
}
int add(int a,int b){
    return a+b;
}
int main(){
    sayHello();
    cout << add(5,3) << endl;
}
