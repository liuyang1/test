int baz() {
#if EXTRA
    return 0xa + 1;
#else
    return 0xa;
#endif
}
