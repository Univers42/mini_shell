#ifndef BUILTINS_H
#define BUILTINS_H
#include "libft.h"

// here we would use bit masks
typedef enum
{
	ECHO_N = 1 << 0,
	EXPORT_P = 1 << 1,
	UNSET_V = 1 << 2,
	UNSET_F = 1 << 3,
	CD_L = 1 << 4,
	CD_P = 1 << 5,
	FLAG_COUNT = 6
}t_builtin_flags;

typedef struct 
{
	t_string	name;
	int		(*builtin)(char **args, int flags, t_env **env);
	int		valid_flags;
}			t_builtin;

typedef int (*builtin)(char **args, int flags, t_env *env);

builtin	echo(char **args, int flags, t_env *env);
buitin	cd(char **args, int flags, t_env *env);
builtin	pwd(char **args, int flags, t_env *env);
builtin	export(char **args, int flags, t_env *env);
builtin	unset(char **args, int flags, t_env *env);
builtin env(char **args, int flags, t_env *env);
builtin	exit(char **args, int flags, t_env *env);

#endif
