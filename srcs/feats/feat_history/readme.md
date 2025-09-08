# HISTORY
 > When we start a new minishell session it loads the previous history from the file, so arrow-up and the history builtin show commands from previous runs.
 
## Reasoning 
- We only use the allowed readline functions : `readline`, `add_history` , `rl_clear_history` , `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`
- We do not use any forbidden readline/history functions (like `using_history`, `read_history`, `write_hitory`, `rl_redisplay`, `read_)
to make history persistent accross different minishell sessions, we need to:
1. Save history to a file when the shell exits
- this is done by calling `api_save()` `(or hs()->save())` during shutdown
- it writes all history entries from our internal list to a file (default: ~/.minishell_history).

# load history from the file when the shells starts
This is don eby calling the api_load() after initialization
it reads each line from the history file and adds it to our internal list an dto readline 


# In the nutshell
- History is persistent because it it saved to and loaded from a file between sessions
- The file is updated on exit and read on startup.
- The relevant code is in our api_load and `api_save` functions.