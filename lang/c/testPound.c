// gcc -E testPound.c

// general macro
// string replace
#define o(x) printf("%d", x);
// one pound macro
// stringify
// string replace to literal string, obj -> "obj"
#define a(x) printf("%d", #x);
// double pounds macro
// string concat,  obj -> Fkobj, without whitespace
#define b(x) printf("%d", FK##x);
//
// double pounds using concat, so only produce a single token,
// result must be a valid SINGLE token, so this style is error
// error like this: pasting XX and XX does not give a valid preprocessing token

int main(int argc, char *argv[])
{
    o(obj);
    a(obj);
    b(obj);
    return 0;
}
