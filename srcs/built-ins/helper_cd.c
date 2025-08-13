/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:11:14 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 13:13:19 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

static char *get_env_value(char **envp, const char *key);
static void update_env_var(char **envp, const char *key, const char *value);
static char *resolve_path(const char *path, char **envp);
void    update_pwd_vars(char **envp const char *old_pwd);
