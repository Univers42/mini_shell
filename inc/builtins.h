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

t_builtin       gbuiltin()                 {
        static t_builtin a[] =
        {
                {"echo", echo, ECHO_N};
                {"cd", cd};
                {"pwd", pwd, 0};
                {"export", export, 0};
                {"unset", unset, };
                {"exit", exit, 0};
                {NULL, NULL, 0};
        };

        return (&a);
}

typedef enum e_cmd_idx
{
	BIN_ECHO = 0,
	BIN_CD,
	BIN_PWD,
	BIN_EXPORT,
	BIN_UNSET,
	BIN_ENV,
	BIN_EXIT,
	BIN_COUNT
}	t_cmd_idx;
#endif
