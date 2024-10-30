#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MY_SYSCALL_NUMBER 548 // depend of your syscall

int main()
{
    printf("ECOLE 2600 MY INIT\n");
    syscall(MY_SYSCALL_NUMBER);
    system("insmod hacking/first_module/w3.ko");
    while (2600)
    {
        pid_t pid = fork();
        int status = 0;
        if (pid)
        {
            waitpid(pid, &status, 0);
            printf("Respawn\n");
            pid = 0;
        }
        else
        {
            char *tab[] = {"/usr/bin/setsid", "cttyhack", "sh", NULL};
            execv("/usr/bin/setsid", tab);
        }
    }
}
