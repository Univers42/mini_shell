#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ft_scanf.h"

/* Demo program showing how to use fd-based & exec-enabled ft_fscanf helpers
   Useful patterns for a minishell:
   - capture shell-expanded output (ft_fscanf_sh)
   - run a program directly and capture its stdout (ft_fscanf_exec)
   - scan from any file descriptor (ft_fscanf_fd) -> works with pipes/files
   - parse integers with automatic base detection using %i
*/

int main(void)
{
    char word[256];
    int  num;
    int  ret;

    extern char **environ;

    /* 1) Shell-run command: gets environment expansion ($USER), pipes, etc. */
    ret = ft_fscanf_sh("printf '%s %d\\n' \"$USER\" 42", "%s %d", word, &num);
    if (ret == 2)
        printf("[sh] scanned: '%s' %d\n", word, num);
    else
        printf("[sh] failed (ret=%d)\n", ret);

    /* 2) Direct exec of a binary (no shell expansion) - use /usr/bin/printf to format output */
    char *argv_exec[] = { "/usr/bin/printf", "exec_%s %d\\n", "direct", "99", NULL };
    ret = ft_fscanf_exec("/usr/bin/printf", argv_exec, environ, "%s %d", word, &num);
    if (ret == 2)
        printf("[exec] scanned: '%s' %d\n", word, num);
    else
        printf("[exec] failed (ret=%d)\n", ret);

    /* 3) Read from an arbitrary FD (here: create a pipe, write into it, scan read-end) */
    int pipefd[2];
    if (pipe(pipefd) == 0)
    {
        const char *payload = "fd_line 777\n";
        write(pipefd[1], payload, strlen(payload));
        close(pipefd[1]); /* close writer so reader sees EOF */
        ret = ft_fscanf_fd(pipefd[0], "%s %d", word, &num);
        close(pipefd[0]);
        if (ret == 2)
            printf("[fd ] scanned: '%s' %d\n", word, num);
        else
            printf("[fd ] failed (ret=%d)\n", ret);
    }
    else
        perror("pipe");

    /* 4) Demonstrate %i auto-detects hex/octal/decimal */
    ret = ft_fscanf_sh("printf '0xff 0755\\n'", "%i %i", &num, &ret);
    if (ret == 2 || ret == 1) /* we reused ret variable for second scanned value */
    {
        int hexval = num;
        int octval = ret; /* careful: ret now holds second scanned integer */
        printf("[i  ] parsed hex 0xff -> %d, octal 0755 -> %d\n", hexval, octval);
    }
    else
        printf("[i  ] failed parsing %i (ret=%d)\n", ret, ret);

    return 0;
}
