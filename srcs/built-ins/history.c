#include "builtins.h"
#include "history.h"

/*
 print history list, indexed starting at 1
 uses singleton hs()->dump() which returns malloc'ed NULL-terminated array
*/
int	bin_history(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	char **arr;
	size_t i;

	arr = hs()->dump();
	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		ft_printf("%5zu  %s\n", i + 1, arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
