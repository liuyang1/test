// mtime: 2012-10-30 11:41:49
// auth: liuyang 
//
// 验证位域的处理方法

struct Foo
{
	unsigned int Head:4;
	unsigned int Tail:4;
	unsigned int TT:8;
};

int main(){
	struct Foo foo;
	foo.Head=4;
	foo.Tail=8;
	foo.TT=5;
	printf("%X\n",foo);//84
	printf("%d\n",foo);//132
	printf("%4lX\n",foo);
	return 0;
}
