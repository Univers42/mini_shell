#include "builtins.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:48:38 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/14 10:40:22 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
** alloc_env_without_index
** -----------------------
** Purpose:
**   Allocate a new `char **` big enough to hold:
**     - either a COPY of the entire env (when skip_idx is out of range),
**     - or the env WITHOUT the entry at skip_idx (when skip_idx is valid),
**   PLUS its trailing NULL terminator.
** Notes:
**   - The caller must fill this array (copy strings) and set the final NULL.
**   - The caller is responsible for freeing on later errors.
*/
static char	**alloc_env_without_index(char **envp, int skip_idx)
{
	int		n;
	char	**new_env;

	n = env_len(envp);
	if (skip_idx >= 0 && skip_idx < n)
		new_env = malloc(sizeof(*new_env) * n);
	else
		new_env = malloc(sizeof(*new_env) * (n + 1));
	if (!new_env)
		return (NULL);
	return (new_env);
}

/*
** copy_env_without_index
** ----------------------
** Purpose:
**   Deep‑copy (ft_strdup) the source env array into `dst`, skipping the
**   entry at skip_idx (if skip_idx is valid). The result is compacted (no
**   holes) and NULL‑terminated.
**
** Memory safety:
**   - On strdup failure, already‑copied strings in `dst[0..j-1]` are freed
**     via `free_env_partial(dst, j)`, and the function signals failure.
**
** Params:
**   dst      -> Destination array (already allocated by caller).
**   src      -> Source env array (NULL‑terminated).
**   skip_idx -> Index to skip; if invalid, copies everything.
**
** Returns:
**   0 -> Success; dst is now a valid, NULL‑terminated deep copy (minus skip).
**   1 -> Allocation failure; partial copy has been freed.
**
** Complexity:
**   O(m * s), where m = env entries, s = average string length duplicated.
*/
static int	copy_env_without_index(char **dst, char **src, int skip_idx)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (i != skip_idx)
		{
			dst[j] = ft_strdup(src[i]);
			if (!dst[j])
			{
				free_env_partial(dst, j);
				return (1);
			}
			j++;
		}
		i++;
	}
	dst[j] = NULL;
	return (0);
}

/*
** remove_env_var
** --------------
** Purpose:
**   Remove the env entry at `idx` by building a new env array without that
**   entry, freeing the old array (strings + pointer array), and swapping
**   the pointer. This approach guarantees a compact, leak‑free result.
**
** Why rebuild instead of shifting in place:
**   - Clear ownership semantics (every entry in new array is freshly dup’d).
**   - Easier to ensure no leaks and a correct NULL terminator.
**   - Works uniformly even if later we change how env is stored.
**
** Params:
**   penv -> Pointer to the modifiable `char **` environment (e.g., &env->envp).
**   idx  -> Index to remove (must be >= 0 and < current length).
**
** Returns:
**   0 -> Success (env replaced).
**   1 -> Allocation or copy failure; original env remains untouched.
**
** Side effects:
**   On success, frees all strings in the old env and the old pointer array.
**   On failure, leaves *penv unchanged.
*/
static int	remove_env_var(char ***penv, int idx)
{
	char	**new_env;

	new_env = alloc_env_without_index(*penv, idx);
	if (!new_env)
		return (1);
	if (copy_env_without_index(new_env, *penv, idx))
		return (1);
	free_env_partial(*penv, env_len(*penv));
	*penv = new_env;
	return (0);
}

/*
** bin_unset
** ---------
** Purpose:
**   Minishell builtin implementation for `unset` (subject: “unset with no
**   options”). Removes variables from the shell environment. Silent on
**   success, prints errors for invalid identifiers, and returns a status
**   code compatible with Bash:
**     - 0 if all provided names were valid (regardless of existence).
**     - 1 if at least one provided name was an invalid identifier.
**
** Behavior:
**   - Ignores `flags` (subject requires no options).
**   - For each arg:
**       * If invalid identifier -> print error to STDERR and continue.
**       * Else find exact "NAME=" entry -> if found, remove it.
**   - Environment is updated in the parent process (no fork) so changes
**     persist for subsequent commands.
**
** Params:
**   args  -> Arg vector: ["unset", "NAME1", "NAME2", ... , NULL]
**   flags -> Ignored (kept for builtin signature consistency).
**   env   -> Shell state holding the modifiable `char **envp`.
**
** Returns:
**   0 or 1 as described above.
**
** Complexity:
**   O(k * (m + s)) roughly, where:
**     k = number of names to unset,
**     m = current env entries,
**     s = avg string length duplicated during rebuilds.
**   (Each removal rebuilds once; still fine given typical env sizes.)
**
** Notes:
**   - No output on successful removals (Bash parity).
**   - If a name doesn’t exist, that’s not an error.
**   - Error message format follows common minishell style.
*/
int	bin_unset(char **args, int flags, t_env *env)
{
	int	status;
	int	idx;
	int	i;
	char	**envp;

	(void)flags;
	status = 0;
	i = 1;
	envp = *(char ***)env;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
		{
			idx = find_var_index(envp, args[i], ft_strlen(args[i]));
			if (idx >= 0)
			{
				remove_env_var((char ***)env, idx);
				envp = *(char ***)env;
			}
		}
		i++;
	}
	return (status);
}

