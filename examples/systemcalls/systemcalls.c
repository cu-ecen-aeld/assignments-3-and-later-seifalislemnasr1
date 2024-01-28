#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

bool do_system(const char *cmd)
{
    int stat = system(cmd);
    if (stat == -1)
    {
        perror("Error occurred");
        return false;
    }
    else
    {
        printf("Command executed with exit status %d", stat);
        return true;
    }
    return true;
}

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    const char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, const char *);
    }
    command[count] = NULL;

    pid_t son_pid;
    son_pid = fork();
    if (son_pid == -1)
    {
        perror("Error while forking");
        return false;
    }
    else if (son_pid == 0)
    {
        if (execv(command[0], (char *const *)command) == -1)
        {
            perror("Execv failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        if (waitpid(son_pid, &status, 0) == -1)
        {
            perror("Wait failed");
            return false;
        }
    }

    va_end(args);
    return true;
}

bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    const char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, const char *);
    }
    command[count] = NULL;

    int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Failed opening specified file");
        return false;
    }

    pid_t sonpid;
    sonpid = fork();
    switch (sonpid)
    {
    case -1:
        perror("Error Forking");
        return false;
    case 0:
    {
        char *sous[count + 1];
        for (int i = 0; i < count; i++)
        {
            sous[i] = (char *)command[i + 1];
        }
        sous[count] = NULL;

        if (dup2(fd, 1) < 0)
        {
            perror("Dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);

        if (execvp(command[0], sous) == -1)
        {
            perror("Execvp failed");
            exit(EXIT_FAILURE);
        }
    }
    default:
    {
        close(fd);
        int status;
        if (waitpid(sonpid, &status, 0) == -1)
        {
            perror("Wait failed");
            return false;
        }
    }
    }

    va_end(args);
    return true;
}

int main()
{
    // You can add test cases or use the functions in your program here
    return 0;
}
