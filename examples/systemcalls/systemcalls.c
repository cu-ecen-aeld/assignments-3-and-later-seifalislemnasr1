#include <stdlib.h>
#include "systemcalls.h"
<<<<<<< HEAD
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
=======
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

>>>>>>> 94ba1a6 (nami)

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
<<<<<<< HEAD
    int res = system(cmd);
    bool succes = res != -1;
    return succes;
=======
	int stat = system(cmd);
	bool success = stat != -1;
	return success;
/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

 
>>>>>>> 94ba1a6 (nami)
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
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
        printf("command[%i]: %s\n", i, command[i]);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    if (count < 1) {
        // gaurd against invalid arguments;
        errno = EINVAL;
        return false;
    }

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
<<<<<<< HEAD

    // refrence https://stackoverflow.com/questions/19099663/how-to-correctly-use-fork-exec-wait
    
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return false;
    }

    // child
    if (pid == 0) {
        execv(command[0], &command[0]);
        perror("exec");
        exit(errno);
    } else {
        int exit_status;
        pid = waitpid(pid, &exit_status, 0);
        if (pid == -1){
            perror("wait");
            return false;
        }
        if (exit_status) {
            printf("child failed\n");
            return false;
        }
    }

    va_end(args);

    return true;
=======
    int son_pid = fork();
    if (son_pid == -1){
        perror("Error while forking");
        return false;
        }
    if (son_pid == 0){
       
        execv(command[0], &command[0]);
    	perror("Execv failed");
    	exit(errno);
    	     }
    else {
        int status;
        if ( waitpid(son_pid,&status,0) == -1){
             perror("Wait");
             return false;            
			}
	if (exit_status) {
            printf("child failed\n");
            return false;		
}
     va_end(args);
     return true;  
}
>>>>>>> 94ba1a6 (nami)
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
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];
    int fd = open("redirected.txt", O_WRONLY|O_TRUNC|O_CREAT, 0644);
    int sonpid;
    if (fd<0){
    	perror("Failed opening specified file");
    	return false;
    	}
    switch (sonpid == fork()) {
    	case -1: perror ("Error Forking");return false;
    	case 0:
    		if (dup2(fd,1) < 0){
    			perror("Dup2");
    			return false;
    			}
    		close(fd);
    		const char **sous = malloc((count + 1) * sizeof(char*));
        	for (int i = 0; i < count; i++) {
             		sous[i] = command[i + 1];
             	}
        	sous[count] = NULL;
    		
    		int stat_ex1 = execvp(command[0],sous);
    		if (stat_ex1 == -1){
    	     		perror("Execvp failed");
    	     		return false;
    	     	}
        default:
        	close(fd);
        	
}
  int status;
  if ( waitpid(son_pid,&status,0) == -1){
       perror("Wait failed");
       return false;
               
}
		
    	

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return false;
    }

    // child
    if (pid == 0) {
        int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
        if (fd == -1){
            perror("open");
            exit(1);
        }
        int fd2 =dup2(fd, 1);
        if (fd2 == -1){
            perror("dup2");
            exit(1);
        }
        execv(command[0], &command[0]);
        perror("exec");
        exit(1);
    } else {
        int exit_status;
        pid = waitpid(pid, &exit_status, 0);
        if (pid == -1){
            perror("wait");
            return false;
        }
        if (exit_status) {
            printf("child failed\n");
            return false;
        }
    }


    va_end(args);

    return true;
}
