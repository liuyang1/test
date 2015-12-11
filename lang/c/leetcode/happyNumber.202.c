int iter(int n)
{
    int sum, rem;
    for (sum = 0; n != 0; n /= 10) {
        rem = n % 10;
        sum += rem * rem;
    }
    return sum;
}

bool isHappy(int n) {
    int next = n, fast = n;
    while (1) {
        next = iter(next);
        fast = iter(iter(fast));
        if (next == 1 || fast == 1) {
            return true;
        }
        if (next == fast) {
            return false;
        }
    }
}
