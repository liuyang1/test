int rmFactor(int num, int p)
{
    while (num % p == 0) {
        num /= p;
    }
    return num;
}

bool isUgly(int num) {
    if (num <= 0) {
        return false;
    }
    num = rmFactor(num, 2);
    num = rmFactor(num, 3);
    num = rmFactor(num, 5);
    return num == 1;
}
