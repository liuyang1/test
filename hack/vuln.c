/*
 * Tao of Hacking Page 17
 * liuyang1@mail.ustc.edu.cn 
 * 2013-11-30 15:08:01 
 *
 * this lead to "Stack smashing detected"
 * because gcc compiled with stack protected
 * so using -fno-stack-protector while compiling
 * to continue this code
 */
int 
main(int argc, char**argv)
{
	char buffer[500];
	strcpy(buffer, argv[1]);
	return 0;
}
