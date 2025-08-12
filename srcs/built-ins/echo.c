#include "builtins.h"

int bin_echo(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("echo builtin called\n");
	return 0;
}
