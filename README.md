# Get Next Line - Reading a line from a fd is way too tedious

### This project is about programming a function that returns a line read from a file descriptor
*Version : 10*

#### Goals
This project will not only allow you to add a very convenient function to your collection,
but it will also make you learn a highly interesting new concept in C programming: static
variables.

---

#### Mandatory part
##### Parameters 
* fd: The file descriptor to read from
* Return value Read line: correct behavior
* NULL: there is nothing else to read, or an error occurred
##### External functs. 
* read, malloc, free
##### Description
* Write a function that returns a line read from a file descriptor

#### Bonus part
* Develop get_next_line() using only one static variable.
* Your get_next_line() can manage multiple file descriptors at the same time.
For example, if you can read from the file descriptors 3, 4 and 5, you should be
able to read from a different fd per call without losing the reading thread of each
file descriptor or returning a line from another fd.
It means that you should be able to call get_next_line() to read from fd 3, then
fd 4, then 5, then once again 3, once again 4, and so forth.

보너스 함수는 기본 함수의 확장이므로, 보너스 함수만 만들었습니다.

이 밑은, 각 함수에 대해서, 간략하게 해당 함수의 코드와 함께 정리한 내용들입니다.
함수의 순서는 호출 순서입니다.
  
<details>
    <summary>char	*get_next_line(int fd)</summary>
  
```c
char	*get_next_line(int fd)
{
	static t_list	*lst;
	t_list			*node;
	char			*ret;

	node = find_fd(fd, &lst);
	if (!node || node->idx < 0)
		return (0);
	if (read(node->fd, 0, 0) < 0)
		return (delete_fd(fd, &lst));
	ret = read_next_string(node, &lst);
	if (ret && ret[0])
		return (ret);
	if (ret)
		free(ret);
	return (0);
}
```
* 인자로 받은 fd가 들어있는 노드를 find_fd를 통해 연결리스트에서 찾습니다.
* fd로부터 파일을 읽을 수 있는지 확인하고, 읽을 수 없다면 해당 노드를 지우고, 아니라면 read_next_string으로 다음 스트링을 받아옵니다.
- - -
</details>

<details>
    <summary>static t_list	*find_fd(int fd, t_list **lst)</summary>

```c
typedef struct s_list
{
	int				fd;
	int				idx;
	char			*string;
	struct s_list	*next;
}t_list;

static t_list	*find_fd(int fd, t_list **lst)
{
	t_list	*ret;

	ret = *lst;
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
	ret->next = *lst;
	ret->string = (char *)malloc(sizeof(char) * 1);
	if (!ret->string)
		return (0);
	ret->string[0] = '\0';
	ret->idx = 0;
	return (*lst = ret);
}
```
* 인자로 받아온 연결리스트에서, 인자로 받아온 fd와 같은 노드를 찾아 리턴합니다.
* 없다면 노드를 새로 만들어서 리턴합니다.
* 버퍼 크기만큼 읽어온 문자열은 계속 string의 뒤에 이어붙입니다.
* idx는 string에 담긴 문자열에서, 몇 번째 문자까지 처리했는지를 기록합니다.
- - -
</details>

<details>
    <summary>static char	*delete_fd(int fd, t_list **lst)</summary>

```c
static char	*delete_fd(int fd, t_list **lst)
{
	t_list	*tmp;
	t_list	*cur;

	tmp = *lst;
	if (tmp->fd == fd)
	{
		*lst = tmp->next;
	}
	else
	{	
		cur = *lst;
		while (cur->next->fd != fd)
			cur = cur->next;
		tmp = cur->next;
		cur->next = tmp->next;
	}
	free(tmp->string);
	free(tmp);
	return (0);
}
```
- - -
</details>

<details>
    <summary>static char	*read_next_string(t_list *node, t_list **lst)</summary>

```c
static char	*read_next_string(t_list *node, t_list **lst)
{
	char	buf[BUFFER_SIZE];
	int		rd_siz;
	int		len;

	rd_siz = read(node->fd, buf, BUFFER_SIZE);
	if (rd_siz < 0)
		return (0);
	if (rd_siz == 0)
	{
		while (node->string[node->idx] && node->string[node->idx + 1] != '\0' \
		&& node->string[node->idx] != '\n')
			node->idx++;
		return (make_next_string(node, lst));
	}
	node->string = ft_custom_strjoin(node->string, buf, rd_siz);
	if (!node->string)
		return (0);
	len = node->idx + rd_siz;
	while (node->idx < len && \
	node->string[node->idx] != '\0' && node->string[node->idx] != '\n')
		node->idx++;
	if (node->idx == len)
		return (read_next_string(node, lst));
	return (make_next_string(node, lst));
}
```
* 가장 중요한, 핵심이 되는 부분입니다.
* rd_siz가 0이라는 것은, node->string에 다음 문자열이 존재한다는 것이므로 문장의 끝인 널 문자나 개행 문자를 찾습니다.
* 그렇지 않다면, 버퍼에서 rd_siz만큼의 문자열을 node->string에 이어붙인 후, 널 문자나 개행 문자를 찾습니다.
* 이 과정에서 널 문자와 개행 문자를 찾지 못한다면, 재귀적으로 같은 함수를 호출하여 버퍼의 크기만큼 읽어옵니다.
* 문장의 끝을 발견하면 make_next_string을 통해 해당 문장을 스트링으로 만들어 리턴합니다.
- - -
</details>

<details>
    <summary>static char	*make_next_string(t_list *node, t_list **lst)</summary>

```c
static char	*make_next_string(t_list *node, t_list **lst)
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
		ret = ft_substr(node->string, 0, node->idx + 1);
		if (!ret)
			return (0);
		delete_fd(node->fd, lst);
	}
	return (ret);
}
```
* 직전의 함수에서 찾았던 문장의 끝이 개행 문자라면, node->string의 첫 번째 글자부터 해당 글자까지 담은 문자열을 리턴합니다.
* 해당 문자열은 다시 사용할 필요가 없으므로, 사용한 부분은 버립니다.
* 직전의 함수에서 찾았던 문장의 끝이 널 문자라면, 더 이상 해당 fd에서 읽어올 수 있는 정보가 없음을 의미합니다.
* node->string 전체를 리턴하고, 해당 node를 삭제합니다.
- - -
</details>

---
![스크린샷 2022-07-25 오후 9 42 31](https://user-images.githubusercontent.com/67845112/180779955-c029d4cc-6685-4471-a089-067e54313c31.png)
![스크린샷 2022-07-25 오후 9 42 02](https://user-images.githubusercontent.com/67845112/180779963-94d6c570-3904-4de4-9b99-beb4604278a4.png)
  
~~i'm so babo~~
