/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:10 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 12:57:05 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdlib.h>

/*
** is_numeric_string
** -----------------
** Purpose:
**   Check if a string represents a valid numeric argument for exit.
**   Handles optional leading +/- and ensures all remaining chars are digits.
*/
static int	is_numeric_string(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** handle_exit_with_code
** --------------------
** Handle exit with numeric argument
*/
static void	handle_exit_with_code(const char *arg)
{
	long	exit_code;

	if (is_numeric_string(arg))
	{
		ft_putendl_fd("exit", 1);
		exit_code = ft_atol(arg);
		exit((unsigned char)exit_code);
	}
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
		ft_putendl_fd("exit", 1);
		exit(2);
	}
}

/*
** bin_exit
** --------
** Purpose:
**   Exits the shell with appropriate status codes based on arguments.
**
** Behavior:
**   - No args: Print "exit", exit with last command status (assumed 0)
**   - One numeric arg: Print "exit", exit with that status (modulo 256)
**   - One non-numeric arg: Print error, exit with status 2
**   - Multiple args: Print error, return 1 (don't exit)
**
** Returns:
**   Only returns if there are too many arguments (returns 1)
**   Otherwise calls exit() and never returns
*/
int	bin_exit(char **args, int flags, t_env *env)
{
	int	argc;

	(void)flags;
	(void)env;
	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	else if (argc == 2)
		handle_exit_with_code(args[1]);
	else
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	return (0);
}
