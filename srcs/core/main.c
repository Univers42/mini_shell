#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "builtins.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    bool run = true;
    char *input;

    while (run)
    {
        input = readline("minishell> ");
	/**
	 * GNU readline trick `CTRL + D` is recognized because
	 * readline() returns NULL when it gets an EOF from standard input
	 * readline reads input characters until it hits a newline
	 * ctrl+D sends the `end-of-file (EOF)` signal to the terminal input stream if 
	 * the line buffer is empty
	 * that means: no character typed, press CTRL+D->EOF
	 * readline()  sees EOF and returns NULL instead of a string
	 * */
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (strlen(input) > 0)
        {
            // Check for builtins
            int idx = -1;
            t_builtins *bins = access_builtins();
            for (int i = 0; bins[i].name != NULL; ++i) {
                if (strcmp(input, bins[i].name) == 0) {
                    idx = i;
                    break;
                }
            }
            if (idx >= 0) {
                // Call builtin (dummy args/flags/env for now)
                bins[idx].builtin(NULL, 0, NULL);
            } else if (strcmp(input, "quit") == 0) {
                run = false;
            } else {
                printf("Unknown command: %s\n", input);
            }
        }
        free(input); // Important: readline mallocs the string
    }
    rl_clear_history();
    return (0);
}
