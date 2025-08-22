#ifndef CORE_UTILS_H
# define CORE_UTILS_H

# include <stdbool.h>
# include "minishell.h"

/* ms context (singleton) */
t_ms	*ms(void);
void	ms_install(t_ms *ptr);

/* readline setup */
void	setup_readline_hooks(void);

/* env tools */
char	**dup_env(char **envp);

/* main loop */
int		run_minishell(bool run, t_env *env, t_ms *app);

#endif
