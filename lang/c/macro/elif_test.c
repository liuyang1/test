int main() {
#ifdef A
#error ("defined A")
#elif B
#error ("defined B")
#else
#error ("neither A, nor B")
#endif
    return 0;
}
