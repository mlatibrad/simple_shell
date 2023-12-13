#include "shell.h"

/**
 * _eputs - prints an input string
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes the character c to stderr
 * Return: On success 1, on error, -1 is returned
 */
int _eputchar(char c)
{
	static int i;
	static char buf[OUTPUT_BUFFER_SIZE];

	if (c == BUF_FLUSH || i >= OUTPUT_BUFFER_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putfd - writes the character c to given fd
 * Return: On success 1, on error, -1 is returned
*/
int _putfd(char c, int fd)
{
	static int i;
	static char buf[OUTPUT_BUFFER_SIZE];

	if (c == BUF_FLUSH || i >= OUTPUT_BUFFER_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putsfd - prints an input string
 * Return: the number of characters put
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd);
	}
	return (i);
}
