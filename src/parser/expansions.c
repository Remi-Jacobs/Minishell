/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:44:20 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/17 18:38:50 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_var_val_cpy(char *new_arg, const char *env_value, int pos)
{
	int	i;

	i = 0;
	while (env_value[i])
		new_arg[pos++] = env_value[i++];
	return (i);
}

static void	ft_insert_var(t_expand_data *data, char *arg, \
t_env_variable *env, int ret)
{
	char	*env_value;

	env_value = ft_get_var_value(arg, data->dest_index, env, ret);
	if (env_value)
		data->src_index += ft_var_val_cpy(data->expanded_str, \
		env_value, data->src_index);
	else
		data->src_index += 0;
	ft_memdel(env_value);
	if (arg[data->dest_index] == '?')
		data->dest_index++;
	if (ft_isdigit(arg[data->dest_index]) == 0 && \
	arg[data->dest_index - 1] != '?')
	{
		while (ft_is_env_char(arg[data->dest_index]) == 1)
			data->dest_index++;
	}
	else
	{
		if (arg[data->dest_index - 1] != '?')
			data->dest_index++;
	}
}

char	*ft_expansions(char *arg, t_env_variable *env, int ret)
{
	t_expand_data	data;
	int				new_arg_len;

	new_arg_len = ft_arg_alloc_len(arg, env, ret);
	data.expanded_str = malloc(sizeof(char) * (new_arg_len + 1));
	if (!data.expanded_str)
		return (NULL);
	data.src_index = 0;
	data.dest_index = 0;
	while (data.src_index < new_arg_len && arg[data.dest_index])
	{
		while (arg[data.dest_index] == EXPANSION)
		{
			data.dest_index++;
			if ((arg[data.dest_index] == '\0' || \
			ft_isalnum(arg[data.dest_index]) == 0) \
			&& arg[data.dest_index] != '?')
				data.expanded_str[data.src_index++] = '$';
			else
				ft_insert_var(&data, arg, env, ret);
		}
		data.expanded_str[data.src_index++] = arg[data.dest_index++];
	}
	data.expanded_str[data.src_index] = '\0';
	return (data.expanded_str);
}
