#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

using namespace std;
/*
int main(int argc, char * argv[])
{
        try{
                Server app;
                app.main(argc, argv);
        }
        catch(...)
        {

                std::cout<<"app error\n"<<std::endl;

        }
        return 0;
}
*/
static pid_t sChildPID = 0;

void StartServer(int argc, char* argv[])
{
// 	Server app;
// 	app.main(argc, argv);//真正的主程序
	std::cout<<"server begin to run"<<std::endl;

	for(int i=0;i<3;i++){
		cout<<"time : "<<i<<" seconds"<<endl;
		sleep(1);
	}
	std::cout<<"cause a seg fault"<<endl;

	*(int *)NULL=0;
//	while(1)
//		;
}

bool sendtochild(int sig, pid_t myPID)
{
	if (sChildPID != 0 && sChildPID != myPID) // this is the parent
	{   // Send signal to child
		::kill(sChildPID, sig);
		return true;
	}

	return false;
}

void sigcatcher(int sig, int , struct sigcontext*)
{
	pid_t myPID = getpid();

	if(sig == SIGTERM || sig == SIGQUIT || sig == 0 || sig == SIGSEGV)
	{
		if (sendtochild(sig,myPID))
		{
			exit(0);
			return;
		}
		else
		{
			exit(4);
			return;
		}
	}
}

int main(int argc, char* argv[])
{
	struct sigaction act;

#if defined(sun) || defined(i386) || defined (__MacOSX__) || defined(__powerpc__) || defined (__osf__) || defined (__sgi_cc__) || defined (__hpux__)
	// 一般使用这一段,其余忽略
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = (void(*)(int))&sigcatcher;//设置回调函数
#elif defined(__sgi__)
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = (void(*)(...))&sigcatcher;
#else
	act.sa_mask = 0;
	act.sa_flags = 0;
	act.sa_handler = (void(*)(...))&sigcatcher;
#endif

	(void)::sigaction(SIGTERM, &act, NULL);
	(void)::sigaction(SIGQUIT, &act, NULL);
	(void)::sigaction(SIGSEGV, &act, NULL);//注册SIGSEGV事件

	pid_t processID = 0;
	int status = 0;
	int pid = 0;

	do
	{
		processID = fork();// 开启子进程
		if (processID > 0) // this is the parent and we have a child
		{
			sChildPID = processID;
			status = 0;
			while (status == 0)
			{
				pid = ::wait(&status);
				signed char exitStatus = (signed char) WEXITSTATUS(status);
				if (WIFEXITED(status) && pid > 0 && status != 0) // child exited with status -2 restart or -
				{
					if ( exitStatus == -1) // child couldn't run don't try again
					{
						exit (EXIT_FAILURE);
					}
					break; // restart the child

				}

				if (WIFSIGNALED(status)) // child exited on an unhandled signal (maybe a bus error or seg fa
				{
					break; // restart the child
				}
				if (pid == -1 && status == 0)
				{
					perror("test");
					exit(0);
					continue;
				}

				if (pid > 0 && status == 0)
				{
					exit(EXIT_SUCCESS);
				}

				exit (EXIT_FAILURE);
			}
		}
		else if (processID == 0) // must be the child
			break;
		else
			exit(EXIT_FAILURE);

		sleep(1);
	} while (1); // fork again based on pref if server dies

	if (processID != 0) //the parent is quitting
		exit(EXIT_SUCCESS);

	StartServer(argc, argv);

	return 0;
}

