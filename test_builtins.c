#include <stdio.h>
#include "builtins.h"
#include "libft/libft.h"

void test_echo(void)
{
    char *args1[] = {"echo", "hello", "world", NULL};
    char *args2[] = {"echo", "-n", "hello", "world", NULL};
    
    printf("Test 1 - Basic echo: \n");
    builtin_echo(args1, STDOUT_FILENO);
    
    printf("Test 2 - Echo with -n flag: \n");
    builtin_echo(args2, STDOUT_FILENO);
}

void test_env(void)
{
    char *args[] = {"env", NULL};
    builtin_env(args, STDOUT_FILENO);
}

void test_export(void)
{
    char *args1[] = {"export", "TEST_VAR=hello", NULL};
    char *args2[] = {"export", NULL};
    
    printf("\n");
    builtin_export(args1, STDOUT_FILENO);
    printf("Printing all variables:\n");
    builtin_export(args2, STDOUT_FILENO);
}

void test_unset(void)
{
    char *args[] = {"unset", "PWD", NULL};
    
    printf("Before unset:\n");
    printf("TEST_VAR=[%s]\n", env_get(args[1]));
    builtin_unset(args);
    printf("After unset:\n");
    printf("TEST_VAR=%s\n", env_get(args[1]));
}

void test_pwd(void)
{
    builtin_pwd(STDOUT_FILENO);
}

void test_cd(void)
{
    char *args1[] = {"cd", "..", NULL};
    char *args2[] = {"cd", "-", NULL};
    
    printf("Current directory before cd:\n");
    builtin_pwd(STDOUT_FILENO);
    
    printf("\nChanging to parent directory:\n");
    builtin_cd(2, args1);
    builtin_pwd(STDOUT_FILENO);
    
    printf("\nChanging back:\n");
    builtin_cd(2, args2);
    builtin_pwd(STDOUT_FILENO);
}

int main(void)
{
    test_echo();
    /*test_env();*/
    /*test_export();*/
    /*test_unset();*/
    /*test_pwd();*/
    /*test_cd();*/
	manager_free_everything();
    
    return 0;
}
