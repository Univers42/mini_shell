/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_meta.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 00:24:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/09 01:21:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_PROMPT_LEN 1024
#define MAX_CMD_LEN 256
#define MAX_VARIABLES 20
#define DFLT_TIMEOUT 1000

/**
 * Advanced features
 * Conditional formatting (different colors based on git status, exit codes)
 * Multi-line prompts with continuation handling
 * Theme system for different prompt styles
 * Performance monitoring to avoid slow prompts
 */

typedef struct s_variable
{
	char *name;
	char *cmd;
	int ttl;
	int is_async;
} t_variable;

typedef struct s_cmd_exec
{
	pid_t pid;
	int pipe_fd;
	char *variable_name;
	char buffer[BUFFER_SIZE];
	size_t buffer_pos;
	int completed;
} t_cmd_exec;

typedef struct s_cache_entry
{
	char *key;
	char *value;
	time_t timestamp;
	int ttl;
} t_cache_entry;

typedef struct s_async_cmd
{
	pid_t pid;
	int pipe_fd;
	char *variable_name;
	char buffer[BUFFER_SIZE];
	size_t buffer_pos;
} t_async_cmd;

typedef struct s_prompt
{
	char *format_string; //
	t_variable *variables[MAX_VARIABLES];
	int variable_count;
	t_cache_entry cache[MAX_VARIABLES];
	int cache_count;
	char *cached_prompt;
	time_t last_update;
	int refresh_interval;
	t_async_cmd *async_commands[MAX_VARIABLES];
	int async_count;
	char **dynamic_commands; // Commands to execute for variables
} t_prompt;

typedef struct s_app
{
	int last_exit_status;
	t_prompt *config;
} t_app;

// char *expand_prompt_variables(const char *format)
//{
//  Built-in variables you might support:
//  %user     -> whoami
//  %host     -> hostname
//  %pwd      -> pwd
//  %branch   -> git branch --show-current 2>/dev/null
//  %time     -> date +%H:%M:%S
//  %jobs     -> jobs | wc -l
//  %status   -> echo $? (last command exit status)
//}

// t_async_cmd *start_async_cmd(const char *cmd, const char *var_name)
// {

// }

// int	poll_async_cmd(t_async_cmd *cmd)
// {

// }

// void	update_readline_prompt(const char *new_prompt)
// {

// }

// void	refresh_prompt_if_needed(t_prompt *config)
// {

// }

// char *get_dynamic_prompt(t_prompt *config)
// {

// }

// void	add_prompt_variable(t_prompt *config, const char *name, const char *cmd, time_t refresh)
// {

// }

char *capture_output(const char *command)
{
	int pipefd[2];
	pid_t pid;
	char *output;
	ssize_t total_read;
	ssize_t bytes_read;
	char buffer[1024];
	int status;

	if (!command)
		return (NULL);
	if (pipe(pipefd) == -1)
		return (perror("pipe failed"), NULL);
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		close(pipefd[1]);
		execl("/bin/sh", "sh", "-c", command, NULL);
		perror("execl failed");
		exit(127);
	}
	close(pipefd[1]);
	output = malloc(BUFFER_SIZE);
	if (!output)
	{
		perror("malloc failed");
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
		return (NULL);
	}
	total_read = 0;
	while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
	{
		if (total_read + bytes_read >= BUFFER_SIZE - 1)
		{
			bytes_read = BUFFER_SIZE - 1 - total_read;
			if (bytes_read <= 0)
				break;
		}
		memcpy(output + total_read, buffer, bytes_read);
		total_read += bytes_read;
	}
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	output[total_read] = '\0';
	if (total_read > 0 && output[total_read - 1] == '\n')
		output[total_read - 1] = '\0';
	return (output);
}

char *minishell_examples()
{
	char *current_user;
	char *hostname;
	char *git_branch;
	char *pwd_short;
	char prompt_buf[256];
	char *result = NULL;
	// Example 1: Get current user for prompt
	current_user = capture_output("whoami");
	// Example 2: Get hostname for prompt
	hostname = capture_output("hostname -s");
	// Example 3: Get git branch if in a git repo
	git_branch = capture_output("git branch --show-current 2>/dev/null");
	// Example 4: Create a dynamic prompt
	if (current_user && hostname)
	{
		pwd_short = capture_output("basename $(pwd)");
		snprintf(prompt_buf, sizeof(prompt_buf), "[%s@%s:%s]"GREEN_TERM"[%s]"RESET_TERM"$ ",
				current_user, hostname, pwd_short ? pwd_short : "unknown", git_branch);
		free(pwd_short);
		result = ft_strdup(prompt_buf);
	}
	free(current_user);
	free(hostname);
	free(git_branch);
	return result;
}

// int main()
// {
// 	printf("Capture Output Function for Minishell\n");
// 	printf("=====================================\n\n");

// 	minishell_examples();

// 	printf("\n=== How to use in your minishell ===\n");
// 	printf("1. Include this capture_output function in your project\n");
// 	printf("2. Use it to get command output for your prompt:\n");
// 	printf("   char *user = capture_output(\"whoami\");\n");
// 	printf("   char *host = capture_output(\"hostname\");\n");
// 	printf("   // Build your prompt string with user and host\n");
// 	printf("   free(user); free(host);\n\n");

// 	return 0;
// }