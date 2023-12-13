#include "shell.h"

/**
 * resetInformation - initializes info_t struct
 */
void resetInformation(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * setInformation - initializes info_t struct
 */
void setInformation(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = tokenizeString(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		substituteAlias(info);
		SubstituteVariables(info);
	}
}

/**
 * releaseInformation - frees info_t struct fields
 */
void releaseInformation(info_t *info, int all)
{
	freeMemory(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			releaseList(&(info->env));
		if (info->history)
			releaseList(&(info->history));
		if (info->alias)
			releaseList(&(info->alias));
		freeMemory(info->environ);
			info->environ = NULL;
		bulkFree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
