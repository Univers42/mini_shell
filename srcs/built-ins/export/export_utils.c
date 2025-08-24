/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:40:38 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/24 16:43:02 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

void	print_exported_line(char *env)
{
	char	*eq_pos;

	eq_pos = ft_strchr(env, '=');
	if (eq_pos)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env, 1);
		ft_putendl_fd("", 1);
	}
}

void	print_sorted_env(char **envp_copy, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		print_exported_line(envp_copy[i]);
		i++;
	}
}

static void	swap_str(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	partition_str(char **array, int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = array[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (strcmp(array[j], pivot) < 0)
		{
			i++;
			swap_str(&array[i], &array[j]);
		}
		j++;
	}
	swap_str(&array[i + 1], &array[high]);
	return (i + 1);
}

void	ft_quick_sort_str(char **array, int low, int high)
{
	int	pivot_index;

	if (low < high)
	{
		pivot_index = partition_str(array, low, high);
		ft_quick_sort_str(array, low, pivot_index - 1);
		ft_quick_sort_str(array, pivot_index + 1, high);
	}
}
