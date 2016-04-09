// get N bits from position P from X
unsigned int getbits(unsigned int x, int p, int n) {
    return (x >> (p + 1 - n)) & ~(~0 << n);
}
// write N bits Y from position P to X
unsigned int setbits(unsigned int x, int p, int n, int y) {

}
