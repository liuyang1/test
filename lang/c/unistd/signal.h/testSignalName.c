#include <signal.h>
#include <stdio.h>

#define showSignalName(x)  printf("%2d: %s\n", x, # x);

int main() {
    showSignalName(SIGHUP);
    showSignalName(SIGINT);
    showSignalName(SIGQUIT);
    showSignalName(SIGILL);
    showSignalName(SIGTRAP);
    showSignalName(SIGABRT);
    showSignalName(SIGBUS);
    showSignalName(SIGFPE);
    showSignalName(SIGKILL);
    showSignalName(SIGUSR1);
    showSignalName(SIGSEGV);
    showSignalName(SIGUSR2);
    showSignalName(SIGPIPE);
    showSignalName(SIGALRM);
    showSignalName(SIGTERM);
    // showSignalName(SIGSTKFLT);
    showSignalName(SIGCHLD);
    showSignalName(SIGCONT);
    showSignalName(SIGSTOP);
    showSignalName(SIGTSTP);
    showSignalName(SIGTTIN);
    showSignalName(SIGTTOU);
    showSignalName(SIGURG);
    showSignalName(SIGXCPU);
    showSignalName(SIGXFSZ);
    showSignalName(SIGVTALRM);
    showSignalName(SIGPROF);
    showSignalName(SIGWINCH);
    showSignalName(SIGPOLL);
    // showSignalName(SIGPWR);
    return 0;
}
