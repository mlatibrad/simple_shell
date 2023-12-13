#include "shell.h"

/**
 * checkChain - test if current char in buffer is a chain delimeter
 * Return: 1 if chain delimeter, 0 otherwise
 */
int checkChain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = COMMAND_ORR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = COMMAND_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_buf_type = COMMAND_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * validateChain - checks we should continue chaining based on last status
 * Return: Void
 */
void validateChain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == COMMAND_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buf_type == COMMAND_ORR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * substituteAlias - replaces an aliases in the tokenized string
 * Return: 1 if replaced, 0 otherwise
 */
int substituteAlias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_startsWith(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * SubstituteVariables - replaces vars in the tokenized string
 * Return: 1 if replaced, 0 otherwise
 */
int SubstituteVariables(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			substituteString(&(info->argv[i]),
					_strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[i], "$$"))
		{
			substituteString(&(info->argv[i]),
					_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_startsWith(info->env, &info->argv[i][1], '=');
		if (node)
		{
			substituteString(&(info->argv[i]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		substituteString(&info->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * substituteString - replaces string
 * Return: 1 if replaced, 0 otherwise
 */
int substituteString(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
