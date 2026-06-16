#include "systemcalls.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if successful, false otherwise
 */
bool do_system(const char *cmd)
{
    int ret = system(cmd);

    if (ret == -1)
    {
        return false;
    }

    if (WIFEXITED(ret) && (WEXITSTATUS(ret) == 0))
    {
        return true;
    }

    return false;
}

/**
 * Execute a command using fork(), execv(), and waitpid()
 */
bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);

    char *command[count + 1];

    for (int i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    va_end(args);

    pid_t pid = fork();

    if (pid < 0)
    {
        return false;
    }

    if (pid == 0)
    {
        execv(command[0], command);
        exit(EXIT_FAILURE);
    }

    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        return false;
    }

    if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
    {
        return true;
    }

    return false;
}

/**
 * Execute a command redirecting stdout to outputfile
 */
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);

    char *command[count + 1];

    for (int i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    va_end(args);

    pid_t pid = fork();

    if (pid < 0)
    {
        return false;
    }

    if (pid == 0)
    {
        int fd = open(outputfile,
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

        if (fd < 0)
        {
            exit(EXIT_FAILURE);
        }

        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd);

        execv(command[0], command);

        exit(EXIT_FAILURE);
    }

    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        return false;
    }

    if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
    {
        return true;
    }

    return false;
}