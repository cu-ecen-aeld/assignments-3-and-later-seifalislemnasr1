#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "systemcalls.h"  // Assuming you have a header file with declarations for your functions

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
 */
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
        printf("Command executed with exit status %d\n", stat);
        return true;
    }
}

/**
 * @param count -The numbers of variables passed to the function. The variables are command to execute.
 *   followed by arguments to pass to the command
 *   Since exec() does not perform path expansion, the command to execute needs
 *   to be an absolute path.
 * @param ... - A list of 1 or more arguments after the @param count argument.
 *   The first is always the full path to the command to execute with execv()
 *   The remaining arguments are a list of arguments to pass to the command in execv()
 * @return true if the command @param ... with arguments @param arguments were executed successfully
 *   using the execv() call, false if an error occurred, either in invocation of the
 *   fork, waitpid, or execv() command, or if a non-zero return value was returned
 *   by the command issued in @param arguments with the specified arguments.
 */
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
        char *sous[count + 1];
        for (int i = 0; i < count; i++)
        {
            sous[i] = (char *)command[i + 1];
        }
        sous[count] = NULL;

        int stat_ex = execv(command[0], sous);
        if (stat_ex == -1)
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

/**
 * @param outputfile - The full path to the file to write with command output.
 *   This file will be closed at completion of the function call.
 * All other parameters, see do_exec above
 */
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
    switch (sonpid = fork())
    {
    case -1:
        perror("Error Forking");
        return false;
    case 0:
        if (dup2(fd, 1) < 0)
        {
            perror("Dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);

        char *sous[count + 1];
        for (int i = 0; i < count; i++)
        {
            sous[i] = (char *)command[i + 1];
        }
        sous[count] = NULL;

        int stat_ex1 = execvp(command[0], sous);
        if (stat_ex1 == -1)
        {
            perror("Execvp failed");
            exit(EXIT_FAILURE);
        }

    default:
        close(fd);
        int status;
        if (waitpid(sonpid, &status, 0) == -1)
        {
            perror("Wait failed");
            return false;
        }
    }

    va_end(args);
    return true;
}


int main()
{
    // Example usage:
    do_system("ls -l");
    do_exec(3, "/bin/echo", "Hello", "World");
    do_exec_redirect("output.txt", 3, "/bin/echo", "Redirected", "Output");

    return 0;
}
