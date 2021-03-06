#include "minishell.h"

bool	export_valcheck(char *str)
{
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	while (*str != '\0' && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_' && *str != '+' && *str != '=')
			return (false);
		if (*str == '+' && *(str + 1) != '=')
			return (false);
		str++;
	}
	return (true);
}

char	**ft_split_2(char *str, char c)
{
	char	**arr;
	char	*word;
	int		sign;

	arr = malloc(sizeof(char *) * 3);
	if (arr == NULL)
		return (NULL);
	sign = 0;
	while (str[sign] != c)
		sign++;
	word = ft_substr(str, 0, sign);
	arr[0] = word;
	word = ft_substr(str, sign + 1, strlen(str));
	arr[1] = word;
	arr[2] = NULL;
	return (arr);
}

void 	redir_streams(const int *in, const int *out)
{
	if (dup2(*in, STDIN_FILENO) == -1)
		exit(cmd_err("dup2", "stdin", strerror(errno)));
	if (dup2(*out, STDOUT_FILENO) == -1)
		exit(cmd_err("dup2", "stdout", strerror(errno)));
}

int	exec_set__ext(t_dict **envp, char *arg)
{
	char	*key;

	if (*envp == NULL)
		*envp = dict_newentry(ft_strdup("_"), NULL);
	if (dict_isin(envp, "_"))
		dict_chval(envp, "_", arg);
	else
	{
		key = ft_strdup("_");
		if (!key)
			return (cmd_err(SHELL, "variable _", strerror(errno)));
		else
			dict_addentry(envp, dict_newentry(key, arg));
	}
	return (0);
}

int	exec_set_(t_dict **envp, t_cmd **cmd, int size)
{
	char	*arg;
	t_cmd	*last;
	int		i;

	last = cmd[size - 1];
	if (!last->arg)
		return (1);
	i = (int)strlen_2(last->arg);
	if (i == 0)
		return (1);
	arg = ft_strdup(last->arg[i - 1]);
	if (!arg)
		return (cmd_err(SHELL, "variable _", strerror(errno)));
	if (exec_set__ext(envp, arg))
		return (1);
	return (0);
}
