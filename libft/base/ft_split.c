/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 00:40:52 by rrochd            #+#    #+#             */
/*   Updated: 2024/10/22 10:12:45 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static size_t	count_words(char const *str, char delimeter)
{
	size_t	count;
	size_t	in_word;

	count = 0;
	in_word = 0;
	while (*str != '\0')
	{
		if (*str == delimeter)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static void	ft_free(char **strings)
{
	size_t	i;

	i = 0;
	while (strings[i])
	{
		free(strings[i]);
		i++;
	}
	free(strings);
}

char	**ft_split(char const *str, char c)
{
	char		**strings;
	size_t		words_position;
	const char	*start;

	if (!str)
		return (NULL);
	strings = ft_calloc((count_words(str, c) + 1), sizeof(char *));
	if (strings == NULL)
		return (NULL);
	words_position = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			start = str;
			while (*str && *str != c)
				str++;
			strings[words_position] = ft_substr(start, 0, str - start);
			if (strings[words_position++] == NULL)
				return (ft_free(strings), NULL);
		}
	}
	return (strings);
}
