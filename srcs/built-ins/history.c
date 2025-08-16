#include <stdlib.h>
#include "builtins.h"
#include "history.h"

/*
 print history list, indexed starting at 1
 uses singleton hs()->dump() which returns malloc'ed NULL-terminated array
*/
int	bin_history(char **args, int flags, t_env *env)
{
	char	**arr;
	size_t	i;

	(void)args;
	(void)flags;
	(void)env;
	arr = hs()->dump();
	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		/* cast to unsigned for %u to avoid %zu support issues */
		ft_printf("%5u  %s\n", (unsigned)(i + 1), arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
