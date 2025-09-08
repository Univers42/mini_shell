#include "minishell.h"
#include "libft.h"
#include <unistd.h>

void init_shell(char **envp)
{
    (void)envp;
}

int main(int argc, char **argv, char **envp)
{
    bool run;

    run = true;
    if (argc > 2)
        return (ft_fprintf(STDERR_FILENO, "Usage: %s <config_file>\n", argv[0]));
    if (argv[1][0] == 'c')
        return (ft_printf("Command mode triggered"));
    while (run)
    {
        usleep(100000);
        /*something processed*/
    }
    init_shell(envp);
    return (0);
}
