#include <stdio.h>
#include "user.h"
#include "userset.h"
#include "action.h"
#include "train.h"
#include <time.h>

int main()
{
	time_t t1=time(NULL);
	//testUser();
	//testAction();
	//testUserSet();
	//testActionSet();
	testTrain();
	time_t t2=time(NULL);
	printf("time: %f\n",(t2-t1)/1.0);
	return 0;
}
