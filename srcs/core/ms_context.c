/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_context.c                                       :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* singleton storage */
static t_ms	**ms_slot(void)
{
	static t_ms	*slot = NULL;

	return (&slot);
}

t_ms	*ms(void)
{
	return (*ms_slot());
}

void	ms_install(t_ms *ptr)
{
	*ms_slot() = ptr;
}
