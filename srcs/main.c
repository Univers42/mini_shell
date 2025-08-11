#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
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
        if (strcmp(input, "quit") == 0)
            run = false;

        free(input); // Important: readline mallocs the string
    }
    rl_clear_history();
    return (0);
}
