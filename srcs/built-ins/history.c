#include <stdlib.h>
#include "builtins.h"
#include "history.h"

/* trim-leading check for 'history' followed by end or spaces */
static int	is_history_cmd(const char *s)
{
	const char *p;

	if (!s)
		return (0);
	p = s;
	while (*p && ft_isspace((unsigned char)*p))
		p++;
	/* match exact "history" prefix */
	if (p[0] != 'h' || p[1] != 'i' || p[2] != 's' || p[3] != 't'
		|| p[4] != 'o' || p[5] != 'r' || p[6] != 'y')
		return (0);
	p += 7;
	/* allow only trailing spaces after keyword */
	while (*p && ft_isspace((unsigned char)*p))
		p++;
	return (*p == '\0');
}

/*
 print history list, indexed starting at 1
 uses singleton hs()->dump() which returns malloc'ed NULL-terminated array
*/
int	bin_history(char **args, int flags, t_env *env)
{
	char	**arr;
	size_t	i;
	size_t	n;
	size_t	cutoff;

	(void)args;
	(void)flags;
	(void)env;
	arr = hs()->dump();
	if (!arr)
		return (0);

	/* count entries */
	n = 0;
	while (arr[n])
		n++;

	/* hide trailing "history" entries (current invocation and repeated presses) */
	cutoff = n;
	while (cutoff > 0 && is_history_cmd(arr[cutoff - 1]))
		cutoff--;

	/* print up to cutoff, free all */
	i = 0;
	while (i < n)
	{
		if (i < cutoff)
			ft_printf("%5u  %s\n", (unsigned)(i + 1), arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}
