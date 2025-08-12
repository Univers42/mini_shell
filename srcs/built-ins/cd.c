#include "builtins.h"

int bin_cd(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("cd builtin called\n");
	return 0;
}
