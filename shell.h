#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/* for input & output of buffers */
#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 1024
#define BUF_FLUSH -1

/* command chaining */
#define COMMAND_NORMAL	0
#define COMMAND_ORR		1
#define COMMAND_AND		2
#define COMMAND_CHAIN	3

/* for converting number() */
#define CONVERT_TO_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE	".simple_shell_history"
#define MAXIMUM_HISTORY_ENTRIES	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguments to pass into a function
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* hshloop.c */
int hsh(info_t *, char **);
int IDENTIFY_BUILT_IN(info_t *);
void findCommand(info_t *);
void executeCommand(info_t *);

/* parser.c */
int is_cmd(info_t *, char *);
char *duplicateCharacters(char *, int, int);
char *locatePath(info_t *, char *, char *);

/* cmd_exec.c */
int shellLoop(char **);

/* output_handling.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* string_utils.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *startsWith(const char *, const char *);
char *_strcat(char *, char *);

/* string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* string_manipulation */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* string_tokenizer.c */
char **tokenizeString(char *, char *);
char **tokenizeString2(char *, char);

/* realloc.c */
char *_memset(char *, char, unsigned int);
void freeMemory(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memory_handling.c */
int bulkFree(void **);

/* strtol.c */
int interactive(info_t *);
int is_delim(char, char *);
int _isalpha(int);
int _strtol(char *);

/* errors.c */
int _errstrtol(char *);
void displayError(info_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void stripComments(char *);

/* shell_functions.c */
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);

/* builtin.c */
int _myhistory(info_t *);
int _myalias(info_t *);

/* info_handling.c */
void resetInformation(info_t *);
void setInformation(info_t *, char **);
void releaseInformation(info_t *, int);

/* history_operations.c */
char *fetchHistoryFile(info_t *info);
int recordHistory(info_t *info);
int retrieveHistory(info_t *info);
int generateHistoryList(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* env_operations.c */
char *_getenv(info_t *, const char *);
int _myenv(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int initializeEnvList(info_t *);

/* env_management.c */
char **retrieveEnvironment(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* list_management.c */
list_t *appendNode(list_t **, const char *, int);
list_t *appendNode_end(list_t **, const char *, int);
size_t printStringList(const list_t *);
int removeNodeAtIndex(list_t **, unsigned int);
void releaseList(list_t **);

/* input_handling.c */
ssize_t retrieveInput(info_t *);
int _getline(info_t *, char **, size_t *);
void HandleSIGINT(int);

/* list_management1.c */
size_t list_len(const list_t *);
char **convertListToStrings(list_t *);
size_t printGenericList(const list_t *);
list_t *node_startsWith(list_t *, char *, char);
ssize_t findNodeIndex(list_t *, list_t *);

/* variables.c */
int checkChain(info_t *, char *, size_t *);
void validateChain(info_t *, char *, size_t *, size_t, size_t);
int substituteAlias(info_t *);
int SubstituteVariables(info_t *);
int substituteString(char **, char *);

#endif

