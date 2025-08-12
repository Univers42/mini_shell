#include "builtins.h"

int bin_pwd(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("pwd builtin called\n");
	return 0;
}
