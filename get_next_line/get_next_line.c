/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:25:55 by tjo               #+#    #+#             */
/*   Updated: 2022/05/31 15:57:09 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"get_next_line.h"

static t_list	*find_fd(int fd)
{
	static t_list	*lst;
	t_list			*ret;

	ret = lst;
	while (ret)
	{
		if (ret->fd == fd)
			return (ret);
		ret = ret->next;
	}
	ret = (t_list *)malloc(sizeof(t_list));
	if (!ret)
		return (0);
	ret->fd = fd;
	ret->next = lst;
	ret->string = (char *)malloc(sizeof(char) * 1);
	ret->string[0] = '\0';
	ret->idx = 0;
	return (lst = ret);
}

static char	*make_next_string(t_list *node)
{
	char	*ret;
	char	*tmp;

	if (node->string[node->idx] == '\n')
	{
		ret = ft_substr(node->string, 0, node->idx + 1);
		if (!ret)
			return (0);
		tmp = ft_substr(node->string, node->idx + 1, (size_t)(-1));
		if (!tmp)
			return (0);
		free(node->string);
		node->string = tmp;
		node->idx = 0;
	}
	else
	{
		ret = ft_substr(node->string, 0, node->idx);
		if (!ret)
			return (0);
		free(node->string);
		node->idx = -1;
	}
	return (ret);
}

static char	*find_next_string(t_list *node)
{
	if (node->string[node->idx] != '\0')
	{
		while (node->string[node->idx + 1] != '\0' \
		&& node->string[node->idx] != '\n')
			node->idx++;
	}
	return (make_next_string(node));
}

static char	*read_next_string(t_list *node)
{
	char	buf[BUFFER_SIZE];
	int		rd_siz;
	int		len;

	rd_siz = read(node->fd, buf, BUFFER_SIZE);
	if (rd_siz < 0)
		return (0);
	if (rd_siz == 0)
		return (find_next_string(node));
	node->string = ft_custom_strjoin(node->string, buf);
	if (!node->string)
		return (0);
	len = node->idx + rd_siz;
	while (node->idx < len && \
	node->string[node->idx] != '\0' && node->string[node->idx] != '\n')
		node->idx++;
	if (node->idx == len)
		return (read_next_string(node));
	return (make_next_string(node));
}

char	*get_next_line(int fd)
{
	t_list	*node;

	node = find_fd(fd);
	if (!node)
		return (0);
	if (node->idx < 0)
		return (0);
	return (read_next_string(node));
}
