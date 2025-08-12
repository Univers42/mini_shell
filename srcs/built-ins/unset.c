#include "builtins.h"

int bin_unset(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("unset builtin called\n");
	return 0;
}
