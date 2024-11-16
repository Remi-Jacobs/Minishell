/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:15:14 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/15 19:12:08 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * space_alloc - Allocates memory for a new line with extra spaces around separators.
 * 
 * @line: A pointer to the original line of text to be processed.
 * 
 * This function calculates the number of separators in the given `line` and allocates 
 * memory for a new string that will include additional spaces around the separators. 
 * It iterates through the original line to count the separators using `is_sep()`, then 
 * allocates memory accordingly to account for the extra spaces.
 * 
 * Return:
 * - A pointer to the newly allocated string with extra space allocation.
 * - NULL if memory allocation fails.
 */
char	*ft_space_alloc(char *line)
{
	char	*new;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if (ft_is_sep(line, i))
			count++;
		i++;
	}
	if (!(new = malloc(sizeof(char) * (i + 2 * count + 1))))
		return (NULL);
	return (new);
}

/**
 * space_line - Creates a new line with spaces added around separators.
 * 
 * @line: A pointer to the original line of text to be processed.
 * 
 * This function allocates memory for a new line using `space_alloc()`, then iterates 
 * through the original `line`. It checks for quotes and separators, and if a separator 
 * is found outside of quotes, it inserts spaces around it in the new line. Additionally, 
 * it handles cases where the `$` character is not escaped and modifies its representation.
 * 
 * Return:
 * - A pointer to the newly allocated and processed line with spaces around separators.
 * - NULL if memory allocation fails.
 */
// char	*ft_space_line(char *line)
// {
// 	char	*new;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	new = ft_space_alloc(line);
// 	while (new && line[i])
// 	{
// 		if (ft_quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
// 			new[j++] = (char)(-line[i++]);
// 		else if (ft_quotes(line, i) == 0 && ft_is_sep(line, i))
// 		{
// 			new[j++] = ' ';
// 			new[j++] = line[i++];
// 			if (ft_quotes(line, i) == 0 && line[i] == '>')
// 				new[j++] = line[i++];
// 			new[j++] = ' ';
// 		}
// 		else
// 			new[j++] = line[i++];
// 	}
// 	new[j] = '\0';
// 	ft_memdel(line);
// 	return (new);
// }

char	*ft_space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = ft_space_alloc(line);
	while (new && line[i])
	{
    	if (ft_quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
			new[j++] = (char)(-line[i++]);
		else if (ft_quotes(line, i) == 0 && ft_is_sep(line, i))
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			if (line[i - 1] == '>' && line[i] == '>')
				new[j++] = line[i++];
			else if (line[i - 1] == '<' && line[i] == '<')
				new[j++] = line[i++];
			new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	}
	new[j] = '\0';
	ft_memdel(line);
	return (new);
}

int	ft_quote_check(t_shell_state *shell_state, char **line)
{
	if (ft_quotes(*line, 2147483647))
	{
		ft_putendl_fd("syntax error with open quotes", STDERR);
		ft_memdel(*line);
		shell_state->return_code = 2;
		shell_state->active_env = NULL;
		return (1);
	}
	return (0);
}

void	ft_parse_input(t_shell_state *shell_state)
{
	char		*line;
	t_cmd_token	*token;

	signal(SIGINT, &ft_sig_integer);
	signal(SIGQUIT, &ft_sig_exit);
	if (shell_state->return_code)
		ft_putstr_fd("🤬 ", STDERR);
	else
		ft_putstr_fd("😎 ", STDERR);
	line = readline("\033[0;36m\033[1mminishell ▸ \033[0m");
	if (!line)
	{
		shell_state->should_exit = 1;
		ft_putendl_fd("exit", STDERR);
		return ;
	}
	if (*line)
	{
		add_history(line);
		ft_my_add_history(line, shell_state);
	}
	if (global_sig.sigint_received == 1)
		shell_state->return_code = global_sig.last_exit_stat;
	else
		shell_state->return_code = shell_state->return_code;
	if (line && line[0] == '~')
    {
        char *expanded_line = ft_strjoin(shell_state->tilde, line + 1);
        ft_memdel(line);
        line = expanded_line;
    }
	if (ft_quote_check(shell_state, &line))
	{
		ft_memdel(line);
		return ;
	}
	line = ft_space_line(line);
	if (line && line[0] == '$')
		line[0] = (char)(-line[0]);
	shell_state->cmd_list = ft_get_tokens(line);
	ft_memdel(line);
	ft_squish_args(shell_state);
	token = shell_state->cmd_list;
	while (token)
	{
		if (ft_is_type(token, ARG))
			ft_type_arg(token, 0);
		token = token->next;
	}
	shell_state->here_doc_triggered = 0;
}
