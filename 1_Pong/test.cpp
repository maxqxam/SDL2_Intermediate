#include<iostream>
using namespace std;
int main() {
   int a = 10;
   char *intStr = itoa(a);
   string str = string(intStr);
   cout << str;
}