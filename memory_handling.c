#include "shell.h"

/**
 * bulkFree - free a pointer and NULL the address
 * Return: 1 if freed, otherwise 0.
 */
int bulkFree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
