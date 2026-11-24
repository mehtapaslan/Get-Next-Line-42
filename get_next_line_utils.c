/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehaslan <mehaslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 01:19:25 by mehaslan          #+#    #+#             */
/*   Updated: 2026/11/24 03:40:03 by mehaslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i] != '\0')
		i++;
	return (i);
}

char	*gnl_strjoin(char *s1, char const *s2)
{
	size_t			len;
	size_t			i;
	char			*str;
	char			*temp;

	i = 0;
	temp = s1;
	len = gnl_strlen(s1) + gnl_strlen(s2);
	str = (char *)malloc(len + 1);
	if (!str)
	{
		free(temp);
		return (NULL);
	}
	while (s1 && *s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	str[i] = '\0';
	free(temp);
	return (str);
}

char	*gnl_strchr(const char *str, int c)
{
	while (str && *str != (char)c)
	{
		if (!*str)
			return (NULL);
		str++;
	}
	return ((char *)str);
}

void	gnl_free(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}
