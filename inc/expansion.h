// expansion.h
#ifndef EXPANSION_H
#define EXPANSION_H

#include "minishell.h"

// Expansion types
typedef enum e_expansion_type
{
    EXP_VARIABLE,    // $VAR
    EXP_SPECIAL,     // $?, $$, etc.
    EXP_TILDE,       // ~
    EXP_GLOB,        // *, ?
    EXP_BRACE,       // {a,b,c}
    EXP_COMMAND      // $(cmd)
} t_expansion_type;

// Function pointer for expansion handlers
typedef char *(*t_expansion_handler)(char *input, t_env *env);

// Main expansion function
char *expand_string(char *input, t_env *env);

// Individual expansion handlers
char *expand_variables(char *input, t_env *env);
char *expand_special_params(char *input, t_env *env);
char *expand_tilde(char *input, t_env *env);
char *expand_glob(char *input, t_env *env);

#endif

// expansion.c
#include "expansion.h"

// Main expansion orchestrator
char *expand_string(char *input, t_env *env)
{
    char *result;
    char *temp;
    
    if (!input)
        return (NULL);
    
    result = ft_strdup(input);
    
    // Apply expansions in proper order
    // 1. Tilde expansion first
    temp = expand_tilde(result, env);
    free(result);
    result = temp;
    
    // 2. Variable expansion
    temp = expand_variables(result, env);
    free(result);
    result = temp;
    
    // 3. Special parameters
    temp = expand_special_params(result, env);
    free(result);
    result = temp;
    
    // 4. Glob expansion (if needed)
    // temp = expand_glob(result, env);
    // free(result);
    // result = temp;
    
    return (result);
}

// Tilde expansion handler
char *expand_tilde(char *input, t_env *env)
{
    char *result;
    char *home;
    int i = 0;
    
    if (!input || input[0] != '~')
        return (ft_strdup(input));
    
    home = get_env_value(env, "HOME");
    if (!home)
        return (ft_strdup(input)); // Return unchanged if no HOME
    
    // Handle different tilde cases
    if (input[1] == '\0')
    {
        // Just "~"
        return (ft_strdup(home));
    }
    else if (input[1] == '/')
    {
        // "~/something"
        result = malloc(ft_strlen(home) + ft_strlen(input));
        if (!result)
            return (NULL);
        ft_strcpy(result, home);
        ft_strcat(result, input + 1); // Skip the ~
        return (result);
    }
    
    // Other tilde forms (~user, ~+, ~-) can be added here
    return (ft_strdup(input)); // Return unchanged for unsupported forms
}

// Variable expansion handler ($VAR)
char *expand_variables(char *input, t_env *env)
{
    char *result;
    char *temp;
    int i = 0;
    int j = 0;
    int var_start, var_end;
    char *var_name;
    char *var_value;
    
    result = malloc(ft_strlen(input) * 2 + 1); // Rough estimate
    if (!result)
        return (NULL);
    
    while (input[i])
    {
        if (input[i] == '$' && input[i + 1] && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
        {
            // Found variable
            var_start = i + 1;
            var_end = var_start;
            
            // Find end of variable name
            while (input[var_end] && (ft_isalnum(input[var_end]) || input[var_end] == '_'))
                var_end++;
            
            // Extract variable name
            var_name = malloc(var_end - var_start + 1);
            ft_strncpy(var_name, input + var_start, var_end - var_start);
            var_name[var_end - var_start] = '\0';
            
            // Get variable value
            var_value = get_env_value(env, var_name);
            if (var_value)
            {
                ft_strcpy(result + j, var_value);
                j += ft_strlen(var_value);
            }
            
            free(var_name);
            i = var_end;
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    
    // Resize to actual size needed
    temp = ft_strdup(result);
    free(result);
    return (temp);
}

// Special parameters expansion ($?, $$, etc.)
char *expand_special_params(char *input, t_env *env)
{
    char *result;
    char *temp;
    int i = 0;
    int j = 0;
    char buffer[32];
    
    result = malloc(ft_strlen(input) * 2 + 1);
    if (!result)
        return (NULL);
    
    while (input[i])
    {
        if (input[i] == '$')
        {
            if (input[i + 1] == '?')
            {
                // Exit status
                sprintf(buffer, "%d", get_last_exit_status()); // You need this function
                ft_strcpy(result + j, buffer);
                j += ft_strlen(buffer);
                i += 2;
            }
            else if (input[i + 1] == '$')
            {
                // Process ID
                sprintf(buffer, "%d", getpid());
                ft_strcpy(result + j, buffer);
                j += ft_strlen(buffer);
                i += 2;
            }
            else if (input[i + 1] == '0')
            {
                // Shell name
                ft_strcpy(result + j, "minishell"); // Or get from argv[0]
                j += ft_strlen("minishell");
                i += 2;
            }
            else
            {
                result[j++] = input[i++];
            }
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    
    temp = ft_strdup(result);
    free(result);
    return (temp);
}

// Usage example in your parsing
char **expand_command_args(char **args, t_env *env)
{
    char **expanded_args;
    int i = 0;
    int count = 0;
    
    // Count arguments
    while (args[count])
        count++;
    
    expanded_args = malloc((count + 1) * sizeof(char *));
    if (!expanded_args)
        return (NULL);
    
    // Expand each argument
    while (args[i])
    {
        expanded_args[i] = expand_string(args[i], env);
        i++;
    }
    expanded_args[i] = NULL;
    
    return (expanded_args);
}

// Integration example
int execute_command(char **args, t_env *env)
{
    char **expanded_args;
    int result;
    
    // Expand all arguments before execution
    expanded_args = expand_command_args(args, env);
    
    // Execute with expanded arguments
    if (ft_strcmp(expanded_args[0], "cd") == 0)
        result = bin_cd(expanded_args, 0, env);
    else
        result = execute_external(expanded_args, env);
    
    // Cleanup
    free_string_array(expanded_args);
    return (result);
}