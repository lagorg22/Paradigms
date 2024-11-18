#include<stdio.h>


void float_to_text(float n){
    int num = *(int*)&n;
    if(num>>31 && 1)
        printf("-"); 

   int exponent = (char)(num>>23);
    exponent  = exponent - 127;

    printf("1.");
    for(int i = 22; i >= 0; i--){
        if(num>>i & 1) printf("1");
        else printf("0");
    }

    printf(" * 2^%d", exponent);
    
}

int main(){
    float k;
    k = 0.15625;
   // k = 1.5;
    //k = -7.333;

    float_to_text(k);
    return 0;
}