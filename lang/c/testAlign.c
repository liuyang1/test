typedef union {
    int test;
} myunion;
void fn_call(myunion* t)
{

}

int main()
{
    int i = 1;
    fn_call((myunion*)&i);
    return 0;
}
