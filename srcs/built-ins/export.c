#include "builtins.h"

int bin_export(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("export builtin called\n");
	return 0;
}
