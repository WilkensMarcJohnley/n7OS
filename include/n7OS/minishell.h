#ifndef __MINISHELL_H__
#define __MINISHELL_H__

#define MAX_CMD_LEN 256
#define MAX_ARGS 16
#define SHELL_PROMPT "myos> "

// Liste des commandes internes
typedef struct {
    const char *name;
    void (*func)(void);
    const char *help;
} shell_cmd_t;

void shell_run(void);

#endif