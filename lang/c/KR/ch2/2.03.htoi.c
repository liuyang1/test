int hex2int(const char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}
int htoi(const char *s) {
    // skip optional 0x or 0X
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
    }
    unsigned int ret, v;
    const unsigned int base = 16;
    for (ret = 0; (v = hex2int(*s)) != -1; s++) {
        ret = ret * base + v;
    }
    return ret; // cast unsgined to signed
}
