#include "builtins.h"

static char *extract_env_value(const char *name, char **envp)
{
	size_t len = ft_strlen(name);
	size_t i = 0;
	char   *eq;

	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq && (size_t)(eq - envp[i]) == len
			&& ft_strncmp(envp[i], name, len) == 0)
			return (eq + 1);
		i++;
	}
	return (NULL);
}


int	bin_pwd(char **args, int flags, t_env *env)
{
	char	*pwd;
	char	*cwd;

	(void)args;
	(void)flags;
	pwd = extract_env_value("PWD", env->envp);
	if (pwd)
	{
		ft_putendl_fd(pwd, 1);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
