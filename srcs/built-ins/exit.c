#include "builtins.h"

int bin_exit(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("exit builtin called\n");
	return 0;
}
