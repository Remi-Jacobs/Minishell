/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:04:46 by ojacobs           #+#    #+#             */
/*   Updated: 2024/11/07 15:18:21 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_print_err_export(int return_err, const char *arg)
{
	int	i;

	if (return_err == -1)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	else if (return_err == 0 || return_err == -3)
		ft_putstr_fd("not a valid identifier: ", STDERR);
	i = 0;
	while (arg[i] && (arg[i] != '=' || return_err == -3))
	{
		write(STDERR, &arg[i], 1);
		i++;
	}
	write(STDERR, "\n", 1);
	return (ERROR);
}

int	ft_add_to_env(const char *args, t_env_variable *env)
{
	t_env_variable	*new_env;
	t_env_variable	*tmp;

	if (env && env->variable == NULL)
	{
		env->variable = ft_strdup(args);
		return (SUCCESS);
	}
	new_env = malloc(sizeof(t_env_variable));
	if (!new_env)
		return (-1);
	new_env->variable = ft_strdup(args);
	while (env && env->next && env->next->next)
		env = env->next;
	tmp = env->next;
	env->next = new_env;
	new_env->next = tmp;
	return (SUCCESS);
}

char	*ft_get_env_name(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i] && src[i] != '=' && ft_strlen(src) < BUFF_SIZE)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_is_in_env(t_env_variable *env, char *args)
{
	char	var_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	ft_get_env_name(var_name, args);
	while (env && env->next)
	{
		ft_get_env_name(env_name, env->variable);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			ft_memdel(env->variable);
			env->variable = ft_strdup(args);
			return (1);
		}
		env = env->next;
	}
	return (SUCCESS);
}

int	ft_export(char **args, t_env_variable *env, t_env_variable *secret)
{
	int	new_env;
	int	error_ret;

	new_env = 0;
	if (!args[1])
	{
		ft_print_sorted_env(secret);
		return (SUCCESS);
	}
	else
	{
		error_ret = ft_is_valid_env(args[1]);
		if (args[1][0] == '=')
			error_ret = -3;
		if (error_ret <= 0)
			return (ft_print_err_export(error_ret, args[1]));
		if (error_ret == 2)
			new_env = 1;
		else
			new_env = ft_is_in_env(env, args[1]);
		if (new_env == 0)
		{
			if (error_ret == 1)
				ft_add_to_env(args[1], env);
			ft_add_to_env(args[1], secret);
		}
	}
	return (SUCCESS);
}
