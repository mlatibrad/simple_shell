#include "shell.h"

/**
 * hsh - main shell loop
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		resetInformation(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = retrieveInput(info);
		if (r != -1)
		{
			setInformation(info, av);
			builtin_ret = IDENTIFY_BUILT_IN(info);
			if (builtin_ret == -1)
				findCommand(info);
		}
		else if (interactive(info))
			_putchar('\n');
		releaseInformation(info, 0);
	}
	recordHistory(info);
	releaseInformation(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * IDENTIFY_BUILT_IN - finds a builtin command
 * Return: -1 if builtin not found,
 * 	0 if builtin execute successfully,
 * 	1 if builtin found but not successful,
 * 	2 if builtin signal exit()
 */
int IDENTIFY_BUILT_IN(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * findCommand - finds a command in PATH
 * Return: void
 */
void findCommand(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = locatePath(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		executeCommand(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
					|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			executeCommand(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			displayError(info, "not found\n");
		}
	}
}

/**
 * executeCommand - forks an exec thread to run command
 * Return: void
 */
void executeCommand(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, retrieveEnvironment(info)) == -1)
		{
			releaseInformation(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				displayError(info, "Permission denied\n");
		}
	}
}
