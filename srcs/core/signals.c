/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:00:00 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 20:02:34 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "minishell.h"

/*
** Global variable to track if SIGINT (Ctrl+C) was received
** - sig_atomic_t: guaranteed to be atomic (safe to modify in signal handlers)
** - volatile: prevents compiler optimization (value can change unexpectedly)
** - This allows signal handlers to communicate with the main program
*/
volatile sig_atomic_t g_sigint_received = 0;

/*
** Signal handler for SIGINT (Ctrl+C) when shell is in interactive mode
** This function runs when user presses Ctrl+C at the prompt
** 
** @param sig: Signal number (will be SIGINT = 2)
** 
** What this does:
** 1. Set global flag to indicate SIGINT was received
** 2. Write ^C to show user that Ctrl+C was pressed
** 3. Move to new line and redisplay prompt
** 4. Clear current input line in readline
*/
void	ms_handle_sigint_interactive(int sig)
{
	ssize_t	ret;

	(void)sig;  /* We know it's SIGINT, but parameter is required */
	
	/* Set flag so main loop knows SIGINT happened */
	g_sigint_received = 1;
	
	/* 
	** Display ^C without newline - the prompt will handle the newline
	** This ensures immediate visual feedback without extra spacing
	*/
	ret = write(STDOUT_FILENO, "^C", 2);
	(void)ret;
}

/*
** Signal handler for SIGINT when a child process is running
** This is simpler - we just set the flag and let the main loop handle it
** 
** @param sig: Signal number (will be SIGINT = 2)
*/
void	ms_handle_sigint_child(int sig)
{
	ssize_t	ret;
	
	(void)sig;
	
	/* Just set the flag - the main loop will handle killing the child */
	g_sigint_received = 1;
	
	/* Print ^C to show the signal was received */
	ret = write(STDOUT_FILENO, "^C\n", 3);
	(void)ret;  /* Acknowledge we received the return value */
}

/*
** Set up signal handlers for interactive shell mode
** This configures how the shell responds to Ctrl+C and Ctrl+\
** Using sigaction for more reliable signal handling
*/
void	ms_setup_signals(void)
{
	/*
	** Install SIGINT (Ctrl+C) handler for interactive mode
	*/
	signal(SIGINT, ms_handle_sigint_interactive);
	
	/*
	** Ignore SIGQUIT (Ctrl+\) in interactive mode
	*/
	signal(SIGQUIT, SIG_IGN);
}

/*
** Ignore all signals (used when executing child processes)
** Child processes should handle their own signals
*/
void	ms_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*
** Restore default signal behavior
** Used when we want child processes to handle signals normally
*/
void	ms_restore_signals(void)
{
	signal(SIGINT, SIG_DFL);  /* SIG_DFL = default behavior */
	signal(SIGQUIT, SIG_DFL);
}
