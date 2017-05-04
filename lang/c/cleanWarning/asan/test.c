int main(int argc, char **argv) {
    int a[10];
    a[0] = 0;
    return a[10 + argc]; // maybe BOOM
}
