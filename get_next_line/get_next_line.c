/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:25:55 by tjo               #+#    #+#             */
/*   Updated: 2022/05/19 15:25:44 by tjo              ###   ########.fr       */
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
		if (ret->fd_idx == fd)
			return (ret);
		ret = ret->next;
	}
	ret = (t_list *)malloc(sizeof(t_list));
	if (!ret)
		return (0);
	ret->fd_idx = fd;
	ret->next = lst;
	ret->string = (char *)malloc(sizeof(char) * 1);
	ret->string[0] = '\0';
	ret->flag = 0;
	return (lst = ret);
}

static char	*make_nextstring(t_list *node, char *cur)
{
	char	*ret;
	char	*tmp;

	ret = ft_substr(node->string, 0, cur - node->string + 1);
	if (!ret)
		return (0);
	tmp = ft_substr(node->string, cur - node->string + 1, (size_t)(-1));
	if (!tmp)
		return (0);
	free(node->string);
	node->string = tmp;
	return (ret);
}

static char	*read_nextstring(t_list *node, char *cur)
{
	char	buf[BUFFER_SIZE];
	char	*tmp;
	int		read_byte;

	read_byte = read(node->fd_idx, buf, BUFFER_SIZE - 1);
	buf[read_byte] = '\0';
	if (read_byte < 0)
		return (0);
	if (read_byte == 0)
	{
		node->flag = 1;
		return (make_nextstring(node, cur));
	}
	tmp = node->string;
	node->string = ft_strjoin(node->string, buf);
	if (!node->string)
		return (0);
	cur = cur - tmp + node->string;
	free(tmp);
	while (*cur != '\n' && *cur != '\0')
		cur++;
	if (*cur == '\n')
		return (make_nextstring(node, cur));
	return (read_nextstring(node, cur));
}

char	*get_next_line(int fd)
{
	t_list	*node;
	char	*cur;
	char	*ret;

	node = find_fd(fd);
	if (!node)
		return (0);
	cur = node->string;
	while (*cur != '\n' && *cur != '\0')
		cur++;
	if (*cur == '\n')
		return (make_nextstring(node, cur));
	if (node->flag)
	{
		if (node->string[0] == '\0')
			return (0);
		ret = ft_substr(node->string, 0, (size_t)(-1));
		if (!ret)
			return (0);
		free(node->string);
		return (ret);
	}
	return (read_nextstring(node, cur));
}
