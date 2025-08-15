/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scanf.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:56:16 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:54:09 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCANF_H
# define FT_SCANF_H
#include <stdarg.h>
# define BUFFER_SIZE 1024
#ifndef EOF
# define EOF (-1)
#endif

typedef struct s_scanf_data
{
    char    buffer[BUFFER_SIZE];
    int     buf_pos;
    int     buf_len;
    int     fd;
} t_scanf_data;

/* parser function type - keeps your jump table design */
typedef int (*t_scanning)(t_scanf_data *data, va_list args);

/* Core engine */
int         vscanf_internal(const char *format, t_scanf_data *data, va_list args);
int         hexval(int c);

/* Public scanning APIs (FD-based only) */
int         ft_scanf(const char *fmt, ...);
int         ft_vfscanf_fd(int fd, const char *format, va_list args);
int         ft_fscanf_fd(int fd, const char *format, ...);

/* Exec-based helpers (emulate popen+fscanf using pipe/dup2/execve) */
int         ft_fscanf_exec(const char *path, char *const argv[],
                           char *const envp[], const char *format, ...);
int         ft_vfscanf_exec(const char *path, char *const argv[],
                           char *const envp[], const char *format, va_list ap);
int         ft_fscanf_sh(const char *command, const char *format, ...);
int         ft_vfscanf_sh(const char *command, const char *format, va_list ap);

/* Buffer helpers */
void        skip_whitespace(t_scanf_data *data);
void        unget_char(t_scanf_data *data, int c);
int         get_char(t_scanf_data *data);

/* Parsers and jump table */
int         parse_char(t_scanf_data *data, va_list args);
int         parse_string(t_scanf_data *data, va_list args);
int         parse_decimal(t_scanf_data *data, va_list args);
int         parse_integer(t_scanf_data *data, va_list args);
t_scanning  *get_parser_method(void);

#endif