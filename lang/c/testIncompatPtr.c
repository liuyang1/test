int main(void)
{
    // original, totally right
    int ptr = 0;
    unsigned int uiPbufStartUV = ptr + 16;

    void *pStartUV1 = ptr + 16;

    void *pStartUV2 = (unsigned char *)ptr + 16;
    return 0;
}
