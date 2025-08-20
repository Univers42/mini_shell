#include "render.h"
#include "minishell.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int	get_term_cols(void)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
		return ((int)ws.ws_col);
	return (80);
}

void	get_user_str(char *dst, size_t sz)
{
	const char	*user;

	user = getenv("USER");
	if (user)
		ft_strlcpy(dst, user, sz);
	else
		ft_strlcpy(dst, "user", sz);
}

void	get_cwd_str(char *dst, size_t sz)
{
	const char	*home;
	char		tmp[PATH_MAX];

	if (!getcwd(tmp, sizeof(tmp)))
		ft_strlcpy(dst, "?", sz);
	else
	{
		home = getenv("HOME");
		if (home && *home && strncmp(tmp, home, strlen(home)) == 0)
			ft_snprintf(dst, sz, "~%s", tmp + strlen(home));
		else
			ft_strlcpy(dst, tmp, sz);
	}
}
