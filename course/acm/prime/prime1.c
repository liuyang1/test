#include <stdio.h>

int main(){
	int i,j,n;
	n=10000000;
	for(i=7;i<=n;i+=2)
	{
		if(i%2==0)
			continue;
		if(i%3==0)
			continue;
		if(i%5==0)
			continue;
		for(j=2;j*j<i;j++)
		{
			if(i%j==0)
				goto LOOP1_END;
		}
		//printf("%d\n",i);
LOOP1_END:
		;
	}
	return 0;
}
