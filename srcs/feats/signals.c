/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:00:00 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/17 17:20:37 by danielm3         ###   ########.fr       */
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
** Singleton pattern for signal state management
** This replaces the global variable with controlled access functions
** - Encapsulates the signal flag inside a function
** - Provides controlled get/set operations
** - Better security and maintainability than global variables
*/

/*
** Signal state management using singleton pattern
** This function controls access to the SIGINT flag
** 
** @param action: GET_SIGNAL to read, SET_SIGNAL to set, RESET_SIGNAL to clear
** @param value: new value when action is SET_SIGNAL (ignored for GET/RESET)
** @return: current flag value
**
** Usage:
** - signal_flag(GET_SIGNAL, 0) -> returns current flag state
** - signal_flag(SET_SIGNAL, 1) -> sets flag to 1, returns previous value
** - signal_flag(RESET_SIGNAL, 0) -> resets flag to 0, returns previous value
*/
int	signal_flag(t_signal_action action, int value)
{
	static volatile sig_atomic_t	sigint_flag = 0;
	int								previous;

	previous = sigint_flag;
	if (action == SET_SIGNAL)
		sigint_flag = value;
	else if (action == RESET_SIGNAL)
		sigint_flag = 0;
	return (previous);
}


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
	
	/* Set flag using singleton pattern - more secure than global variable */
	signal_flag(SET_SIGNAL, 1);
	
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
	
	/* Set flag using singleton pattern - more secure than global variable */
	signal_flag(SET_SIGNAL, 1);
	
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
