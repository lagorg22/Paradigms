#include <stdio.h>

int power_four(int x) {
    int res = x * x * x * x; // Calculate x to the power of 4

    return res;
}

int process(int x, int y) {
    int a = power_four(x); // Calculate x^4
    int b =  y * y;         // Calculate y^2
    int result = a + b;    // Sum of x^4 and y^2

    return result;
}

int main() {
    int value1 = 2; // First input value
    int value2 = 3; // Second input value

    int res = process(value1, value2); // Call process with value1 and value2

    return res; // Return 0 as the program result
}
