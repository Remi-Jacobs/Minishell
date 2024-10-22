/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 03:30:16 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/22 03:31:00 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

static size_t   ft_env_size(char *env)
{
	size_t size;
	
	size = 0;
	while (env[size] && env[size] != '=')
		size++;
	return (size);
}

static void     ft_remove_env(t_shell_state *state, t_env_variable *env)
{
	if (state->active_env == env && env->next == NULL)
	{
		ft_memdel(state->active_env->variable);
		state->active_env->variable = NULL;
		state->active_env->next = NULL;
		return ;
	}
	ft_memdel(env->variable);
	ft_memdel(env);
}
int		ft_unset(char **args, t_shell_state *state)
{
	t_env_variable	*env;
	t_env_variable	*tmp;

	env = state->active_env;
	if (!args[1])
		return (SUCCESS);
	if (ft_strncmp(args[1], env->variable, ft_env_size(env->variable)) == 0)
	{
		if (state->active_env)
			state->active_env = env->next;
		else
			state->active_env = state->active_env;
		ft_remove_env(state, env);
		return (SUCCESS);
	}
	while (env && env->next)
	{
		if (ft_strncmp(args[1], env->next->variable, ft_env_size(env->next->variable)) == 0)
		{
			tmp = env->next->next;
			ft_remove_env(state, tmp);
			env->next = tmp;
			return (SUCCESS);
		}
		env = env->next;
	}
	return (SUCCESS);
}