/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scanf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 01:00:39 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:54:07 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ft_scanf.h"

/* helpers */

int hexval(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

/* vscanf-like internal engine (FD-based) */
int vscanf_internal(const char *format, t_scanf_data *data, va_list args)
{
    int         matches = 0;
    t_scanning  *parsers;
    int         i;
    int         c;

    matches = 0;
    i = 0;
    parsers = get_parser_method();
    while (format[i])
    {
        if (isspace((unsigned char)format[i]))
        {
            skip_whitespace(data);
            i++;
            continue ;
        }
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '%')
            {
                c = get_char(data);
                if (c == EOF || c != '%')
                {
                    if (c != EOF) unget_char(data, c);
                    break;
                }
                i++;
                continue;
            }
            if ((unsigned char)format[i] && parsers[(unsigned char)format[i]])
            {
                if (parsers[(unsigned char)format[i]](data, args))
                    matches++;
                else
                    break;
            }
            else
                break;
            i++;
        }
        else
        {
            /* literal must match exactly */
            c = get_char(data);
            if (c == EOF || c != (unsigned char)format[i])
            {
                if (c != EOF) unget_char(data, c);
                break ;
            }
            i++;
        }
    }
    return (matches);
}

/* Public FD-based APIs */

int ft_scanf(const char *format, ...)
{
    va_list         args;
    t_scanf_data    data;
    int             matches;

    memset(&data, 0, sizeof(data));
    data.fd = STDIN_FILENO;
    va_start(args, format);
    matches = vscanf_internal(format, &data, args);
    va_end(args);
    return (matches);
}

int ft_vfscanf_fd(int fd, const char *format, va_list args)
{
    t_scanf_data data;
    memset(&data, 0, sizeof(data));
    data.fd = fd;
    return (vscanf_internal(format, &data, args));
}

int ft_fscanf_fd(int fd, const char *format, ...)
{
    va_list     args;
    int         matches;

    va_start(args, format);
    matches = ft_vfscanf_fd(fd, format, args);
    va_end(args);
    return (matches);
}

/* Exec-based ft_fscanf (emulates popen+fscanf without popen/pclose) */

int ft_vfscanf_exec(const char *path, char *const argv[],
                    char *const envp[], const char *format, va_list ap)
{
    int pipefd[2];
    pid_t pid;
    int status = 0;
    int matches;

    if (!path || !format)
        return 0;
    if (pipe(pipefd) == -1)
        return 0;
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return 0;
    }
    if (pid == 0)
    {
        /* Child: connect stdout to pipe and execve */
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            _exit(127);
        close(pipefd[0]);
        close(pipefd[1]);
        execve(path, argv, envp);
        _exit(127);
    }
    /* Parent: read from pipe read-end and scan */
    close(pipefd[1]);
    matches = ft_vfscanf_fd(pipefd[0], format, ap);
    close(pipefd[0]);
    while (waitpid(pid, &status, 0) == -1)
        ; /* ignore */
    (void)status;
    return matches;
}

int ft_fscanf_exec(const char *path, char *const argv[],
                   char *const envp[], const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = ft_vfscanf_exec(path, argv, envp, format, ap);
    va_end(ap);
    return ret;
}

int ft_vfscanf_sh(const char *command, const char *format, va_list ap)
{
    extern char **environ;
    char *argv[4];

    if (!command)
        return 0;
    argv[0] = (char*)"sh";
    argv[1] = (char*)"-c";
    argv[2] = (char*)command;
    argv[3] = NULL;
    return ft_vfscanf_exec("/bin/sh", argv, environ, format, ap);
}

int ft_fscanf_sh(const char *command, const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = ft_vfscanf_sh(command, format, ap);
    va_end(ap);
    return ret;
}