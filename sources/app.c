#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/readline.h>

void run_shell(char **envp)
{
    int size;
    (void)envp;
    char *line = readline("minishell > ");
    if (!line)
    {
        size = write(1, "exit\n", 5);
        (void)size;
        exit(0);
    }
    // process line here if needed
    free(line);
}

int main(int argc, char **argv, char **envp)
{
    bool run = true;
    (void)argc;
    (void)argv;
    while (run)
    {
        run_shell(envp);
    }
    return (0);
}
