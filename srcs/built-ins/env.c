#include "builtins.h"

int bin_env(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("env builtin called\n");
	return 0;
}
