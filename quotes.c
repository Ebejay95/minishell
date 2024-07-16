#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define OR 1
#define AND 2
#define PIPE 3
#define O_PARENT 4
#define C_PARENT 5
#define HERE_DOC 6
#define APPEND 7
#define RE_IN 8
#define RE_OUT 9
#define NL 10

typedef struct s_token
{
    char *value;
    int start;
    int length;
    int is_word;
    struct s_token *next;
} t_token;

typedef struct s_minishell
{
    char *prompt;
    t_token *tok_lst;
} t_minishell;

typedef struct s_mini_unit
{
    int nbr;
    int nbr_sum;
    int lvl;
    char **cmd;
    char *str;
    char **redi;
    int fd_heredoc;
    char ***env_ori;
    int sign_sub;
    struct s_mini_unit *mini;
} t_mini_unit;

typedef struct s_token_expect
{
    int cmd;
    int paren;
    int redi;
} t_token_expect;

t_token *create_token(char *value, int start, int length, int is_word)
{
    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->value = value;
    new_token->start = start;
    new_token->length = length;
    new_token->is_word = is_word;
    new_token->next = NULL;
    return new_token;
}

void add_token_to_list(t_token **list, t_token *token)
{
    if (!*list)
    {
        *list = token;
    }
    else
    {
        t_token *current = *list;
        while (current->next)
            current = current->next;
        current->next = token;
    }
}

char *ft_strndup(const char *s, size_t n)
{
    char *result = (char *)malloc(n + 1);
    if (!result)
        return NULL;
    strncpy(result, s, n);
    result[n] = '\0';
    return result;
}

char *remove_chars(char *str, const char *chars)
{
    char *result = (char *)malloc(strlen(str) + 1);
    if (!result)
        return NULL;
    char *p = result;
    while (*str)
    {
        if (!strchr(chars, *str))
            *p++ = *str;
        str++;
    }
    *p = '\0';
    return result;
}

int ft_isspace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

void print_token_list(t_token *list)
{
    while (list)
    {
        printf("Token: \"%s\", Start: %d, Length: %d, Is Word: %d\n", list->value, list->start, list->length, list->is_word);
        list = list->next;
    }
}

int token(char *str)
{
    if (str[0] == '|' && str[1] == '|')
        return OR;
    if (str[0] == '&' && str[1] == '&')
        return AND;
    if (str[0] == '|')
        return PIPE;
    if (str[0] == '(')
        return O_PARENT;
    if (str[0] == ')')
        return C_PARENT;
    if (str[0] == '<' && str[1] == '<')
        return HERE_DOC;
    if (str[0] == '>' && str[1] == '>')
        return APPEND;
    if (str[0] == '<')
        return RE_IN;
    if (str[0] == '>')
        return RE_OUT;
    return 0;
}

int after_quote(char *str)
{
    int i = 0;
    char quote = str[i];
    if (quote != '"' && quote != '\'')
        return 0;
    i++;
    while (str[i] && str[i] != quote)
    {
        if (str[i] == '\\' && quote == '"' && str[i + 1])
            i++;
        i++;
    }
    return (str[i] == quote) ? i + 1 : i;
}

int ft_take_string(char *str, int *i)
{
    int cnt = 0;
    while (str[*i] && token(str + *i) == 0 && ft_isspace(str[*i]) != 1)
    {
        if (str[*i] == '\'' || str[*i] == '"')
        {
            cnt += after_quote(str + *i);
            *i += after_quote(str + *i);
        }
        else
        {
            cnt++;
            *i += 1;
        }
    }
    return cnt;
}

static int ft_redi(t_mini_unit *mini_unit, char *str, int *i)
{
    int i_old;

    if (token(str + *i) == HERE_DOC || token(str + *i) == APPEND)
        (*i)++;
    (*i)++;
    while (ft_isspace(str[*i]) == 1 && str[*i])
        (*i)++;
    if (str[*i] == 0)
        return 0;
    if (token(str + *i) != 0)
        return 0;
    i_old = *i;
    while (ft_isspace(str[*i]) != 1 && str[*i] != 0 && token(str + *i) == 0)
    {
        *i += after_quote(str + *i);
        if (*i == i_old)
            (*i)++;
        i_old = *i;
    }
    return 1;
}

static int syntax_check_3(t_mini_unit *mini_unit, t_token_expect *tke, char *str, int *i)
{
    if (token(str + *i) == O_PARENT)
    {
        if (tke->cmd != 0)
            return 0;
        mini_unit->mini = (t_mini_unit *)malloc(sizeof(t_mini_unit));
        if (mini_unit->mini == NULL)
            return 0;
        mini_unit->mini->str = ft_strndup(str + *i + 1, after_quote(str + *i) - 2);
        if (mini_unit->mini->str == NULL)
            return 0;
        tke->paren = 1;
        tke->cmd = 1;
        *i += after_quote(str + *i);
        return 1;
    }
    tke->redi = 1;
    if (ft_redi(mini_unit, str, i) == 0)
        return 0;
    return 1;
}

static int syntax_check_2(t_mini_unit *mini_unit, t_token_expect *tke, char *str, int *i)
{
    if (ft_isspace(str[*i]) == 1)
    {
        (*i)++;
        return 1;
    }
    else if (str[*i] == '\'' || str[*i] == '"')
    {
        if (tke->paren == 1)
            return 0;
        tke->cmd = 1;
        *i += after_quote(str + *i);
        return 1;
    }
    else if (token(str + *i) == 0 && str[*i])
    {
        if (tke->paren == 1)
            return 0;
        (*i)++;
        tke->cmd = 1;
        return 1;
    }
    return syntax_check_3(mini_unit, tke, str, i);
}

int syntax_check(t_mini_unit *mini_unit, char *str)
{
    int i = 0;
    int k;
    t_token_expect tke;

    tke.cmd = 0;
    tke.redi = 0;
    tke.paren = 0;
    while (str[i])
    {
        k = syntax_check_2(mini_unit, &tke, str, &i);
        if (k == 0)
            return 0;
        if (k == 2)
            return 1;
    }
    if (tke.cmd == 0 && tke.redi == 0 && tke.paren == 0)
    {
        return 0;
    }
    return 1;
}

int main()
{
    t_minishell m;
    t_mini_unit mini_unit;
    m.prompt = "echo \"This is a test\" 'with single quotes' | cat";
    m.tok_lst = NULL;

    int i = 0;
    int cnt;
    while (m.prompt[i])
    {
        cnt = ft_take_string(m.prompt, &i);
        if (cnt > 0)
        {
            char *token_str = ft_strndup(m.prompt + i - cnt, cnt);
            if (token_str)
            {
                t_token *token = create_token(remove_chars(token_str, "\\"), i - cnt, cnt, 1);
                add_token_to_list(&m.tok_lst, token);
            }
        }
        while (m.prompt[i] && ft_isspace(m.prompt[i]))
            i++;
    }

    print_token_list(m.tok_lst);

    mini_unit.str = m.prompt;
    if (!syntax_check(&mini_unit, m.prompt))
    {
        printf("Syntax error detected.\n");
    }
    else
    {
        printf("No syntax errors.\n");
    }

    return 0;
}
