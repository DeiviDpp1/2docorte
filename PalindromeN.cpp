#include <iostream>
using namespace std;

bool isPalindrome(int x) {
    if(x < 0) return false;
    long invertido=0, num=x;
    while(num>0){
        invertido = invertido*10 + num%10;
        num /= 10;
    }
    return x==invertido;
}
