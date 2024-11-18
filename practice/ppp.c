

int square(int x) {
    int res = x * x;

    return res;
}


int solve(int x, int y) {

    int a = square(x);

    return a + y;
}

int main() {

    int res = solve(3, 5);

    return 0;
}

