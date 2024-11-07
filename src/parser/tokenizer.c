/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:23:49 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/07 16:09:33 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * type_arg - Assigns a type to a command token based on its string content.
 * 
 * @token: A pointer to the token to be analyzed and assigned a type.
 * @separator: An integer indicating whether the token is after a separator (0 or 1).
 * 
 * This function analyzes the string of the given token and assigns an appropriate type 
 * based on the content. It checks if the token matches specific separators like ">", ">>", "<", "|", 
 * or ";". If the token is not after a separator and matches a specific string, it is assigned a type 
 * such as `TRUNC`, `APPEND`, `INPUT`, `PIPE`, or `END`. If the token does not match any of these conditions, 
 * it assigns the token as either a command (`CMD`) or an argument (`ARG`).
 */
void ft_type_arg(t_cmd_token *token, int separator)
{
	if (ft_strcmp(token->content, "") == 0)
		token->type = EMPTY;
	else if (ft_strcmp(token->content, ">") == 0 && separator == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->content, ">>") == 0 && separator == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->content, "<") == 0 && separator == 0)
		token->type = INPUT;
	else if (ft_strcmp(token->content, "|") == 0 && separator == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->content, ";") == 0 && separator == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}

/**
 * squish_args - Reassigns tokens to their appropriate previous command or separator.
 * 
 * @mini: A pointer to the main shell state structure (t_mini).
 * 
 * This function iterates through the linked list of tokens and checks if the current 
 * token is an argument (`ARG`) and if the previous token is of a specific type 
 * (TRUNC, APPEND, or INPUT). If so, it modifies the links in the token list, ensuring 
 * that the argument tokens are associated with their appropriate commands or separators.
 */
void	ft_squish_args(t_shell_state *shell_state)
{
	t_cmd_token	*token;
	t_cmd_token	*prev;

	token = shell_state->cmd_list;
	while (token)
	{
		prev = ft_prev_sep(token, NOSKIP);
		if (ft_is_type(token, ARG) && ft_is_types(prev, "TAI"))
		{
			while (ft_is_last_valid_arg(prev) == 0)
				prev = prev->prev;
			token->prev->next = token->next;
			if (token->next)
				token->next->prev = token->prev;
			token->prev = prev;
			if (prev)
				token->next = prev->next;
			else
				token->next = shell_state->cmd_list;
			if (prev)
				prev = prev;
			else
				prev = token;
			prev->next->prev = token;
			if (shell_state->cmd_list->prev)
				prev->next = prev->next;
			else
				prev->next = token;
			if (shell_state->cmd_list->prev)
				shell_state->cmd_list = shell_state->cmd_list->prev;
			else
				shell_state->cmd_list = shell_state->cmd_list;
		}
		token = token->next;
	}
}

/**
 * next_alloc - Calculates the size needed for the next token string.
 * 
 * @line: A pointer to the line of text being parsed.
 * @i: A pointer to the current index in the line.
 * 
 * This function determines the length needed to allocate memory for the next token 
 * by accounting for special characters, quotes, and escape sequences. It skips spaces 
 * and increments the index accordingly.
 * 
 * Return:
 * - The length of the next token, adjusted for special characters and quotes.
 */
int	ft_next_alloc(char *line, int *i)
{
	int		count;
	int		j;
	char	c;

	count = 0;
	j = 0;
	c = ' ';
	while (line[*i + j] && (line[*i + j] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
			c = line[*i + j++];
		else if (c != ' ' && line[*i + j] == c)
		{
			count += 2;
			c = ' ';
			j++;
		}
		else
			j++;
		if (line[*i + j - 1] == '\\')
			count--;
	}
	return (j - count + 1);
}

/**
 * next_token - Extracts the next token from the line.
 * 
 * @line: A pointer to the line of text being parsed.
 * @i: A pointer to the current index in the line.
 * 
 * This function extracts the next token from the line, handling quotes, escape characters, 
 * and separators. It allocates memory for the token and its string based on the calculated 
 * size from `next_alloc()`. It processes the line character by character, accounting for 
 * quotes and escape sequences.
 * 
 * Return:
 * - A pointer to the newly created token.
 * - NULL if memory allocation fails.
 */
t_cmd_token	*ft_next_token(char *line, int *i)
{
	t_cmd_token	*token;
	int			j;
	char		c;

	j = 0;
	c = ' ';
	token = malloc(sizeof(t_cmd_token));
	token->content = malloc(sizeof(char) * ft_next_alloc(line, i));
	if (!token || !token->content)
		return (NULL);
	while (line[*i] && (line[*i] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
			c = line[(*i)++];
		else if (c != ' ' && line[*i] == c)
		{
			c = ' ';
			(*i)++;
		}
		else if (line[*i] == '\\' && (*i)++)
			token->content[j++] = line[(*i)++];
		else
			token->content[j++] = line[(*i)++];
	}
	token->content[j] = '\0';
	return (token);
}

/**
 * get_tokens - Parses a line into a list of command tokens.
 * 
 * @line: A pointer to the line of text to be parsed.
 * 
 * This function processes a line of text and splits it into a linked list of tokens. 
 * It skips spaces, extracts tokens using `next_token()`, assigns types using `type_arg()`, 
 * and connects the tokens in a linked list. It handles quotes, separators, and other 
 * special characters.
 * 
 * Return:
 * - A pointer to the head of the linked list of tokens.
 */
t_cmd_token	*ft_get_tokens(char *line)
{
	t_cmd_token	*prev;
	t_cmd_token	*next;
	int			i;
	int			sep;

	prev = NULL;
	next = NULL;
	i = 0;
	ft_skip_spacenl(line, &i);
	while (line[i])
	{
		sep = ft_ignore_sep(line, i);
		next = ft_next_token(line, &i);
		next->prev = prev;
		if (prev)
			prev->next = next;
		prev = next;
		ft_type_arg(next, sep);
		ft_skip_spacenl(line, &i);
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	return (next);
}
