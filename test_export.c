#include "inc/builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple test environment
char **create_test_env(void)
{
    char **env = malloc(sizeof(char *) * 4);
    env[0] = strdup("PATH=/bin:/usr/bin");
    env[1] = strdup("HOME=/home/user");
    env[2] = strdup("USER=testuser");
    env[3] = NULL;
    return env;
}

void print_env(char **env)
{
    printf("Current environment:\n");
    for (int i = 0; env[i]; i++)
        printf("  %s\n", env[i]);
    printf("\n");
}

void free_env(char **env)
{
    for (int i = 0; env[i]; i++)
        free(env[i]);
    free(env);
}

int main(void)
{
    char **env = create_test_env();
    char ***penv = &env;
    
    printf("=== TESTING EXPORT BUILTIN ===\n\n");
    
    // Test 1: export without arguments (should print all vars)
    printf("Test 1: export (no arguments)\n");
    char *args1[] = {"export", NULL};
    bin_export(args1, 0, (t_env *)penv);
    printf("\n");
    
    // Test 2: export with new variable
    printf("Test 2: export NEWVAR=hello\n");
    char *args2[] = {"export", "NEWVAR=hello", NULL};
    int result = bin_export(args2, 0, (t_env *)penv);
    printf("Result: %d\n", result);
    print_env(env);
    
    // Test 3: export existing variable
    printf("Test 3: export PATH=/new/path\n");
    char *args3[] = {"export", "PATH=/new/path", NULL};
    result = bin_export(args3, 0, (t_env *)penv);
    printf("Result: %d\n", result);
    print_env(env);
    
    // Test 4: export variable without value
    printf("Test 4: export EMPTYVAR\n");
    char *args4[] = {"export", "EMPTYVAR", NULL};
    result = bin_export(args4, 0, (t_env *)penv);
    printf("Result: %d\n", result);
    print_env(env);
    
    // Test 5: invalid identifier
    printf("Test 5: export 123INVALID=value (should fail)\n");
    char *args5[] = {"export", "123INVALID=value", NULL};
    result = bin_export(args5, 0, (t_env *)penv);
    printf("Result: %d (should be 1)\n", result);
    print_env(env);
    
    // Test 6: export with -p flag
    printf("Test 6: export -p\n");
    char *args6[] = {"export", NULL};
    result = bin_export(args6, FLAG_P, (t_env *)penv);
    printf("Result: %d\n", result);
    
    free_env(env);
    return 0;
}
