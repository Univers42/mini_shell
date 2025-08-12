# minishell

## Project Overview

minishell is a modular shell implementation designed to be highly extensible, maintainable, and robust. The project leverages modern C design patterns and a custom library to provide a flexible command parsing and execution framework.

## Key Features

- **Modular Builtin System:**  
  Builtins are defined using a struct with a name, function pointer, and bitmask for valid flags. This allows easy addition of new builtins and flag types without changing core logic.

- **Bitmask-based Flag Parsing:**  
  Flags for builtins are parsed and validated using bitmasks, enabling efficient and scalable flag handling. Invalid flags are detected early and reported with clear error messages.

- **Lexer/Parser Separation:**  
  Input is tokenized and parsed using a state machine, making the code easy to extend for advanced shell features (quotes, pipes, redirections, etc.).

- **Norminette Compliance & Readability:**  
  Functions are short, modular, and avoid deep nesting, making the code easy to read, maintain, and compliant with coding standards.

- **Extensible API:**  
  All parsing and dispatch logic is exposed via headers, allowing for future expansion and integration with other modules.

- **Robust Error Handling:**  
  The shell provides specific error codes and messages for unknown commands, invalid flags, and empty input, improving user feedback and debugging.

## Why This Approach Is Powerful

- **Scalability:**  
  Adding new builtins, flags, or features only requires updating the bitmask and parser logic, not rewriting core functions.

- **Maintainability:**  
  Clear separation of concerns (lexer, parser, execution) means each part can be improved independently.

- **Extensibility:**  
  The design is ready for advanced shell features, such as environment variable expansion, command substitution, and complex tokenization.

- **Performance:**  
  Bitmask operations and state machines are fast and memory-efficient, suitable for real-world shell workloads.

## Getting Started

- Build with `make`.
- Run `./minishell` and try commands like `echo -n`, `cd -p`, `unset -vf`.
- Extend by adding new builtins or parser features in the respective modules.

## Code Structure
```bash
❯ tree srcs
srcs
├── a.out
├── built-ins
│   ├── cd.c
│   ├── echo.c
│   ├── env.c
│   ├── exit.c
│   ├── export.c
│   ├── pwd.c
│   └── unset.c
├── core
│   └── main.c
├── events
├── execution
│   ├── bin_ctrl.c
│   ├── delim.c
│   ├── pipex.c
│   ├── sym.dlt.c
│   ├── sym_dgt.c
│   ├── sym_gt.c
│   └── sym_lt.c
└── parser
    ├── lexer.c
    └── parser.c

6 directories, 18 files
```

Based on this initialization, each line in the builtins array represents the signature of a builtin command:
```c
/* { <name>, <function pointer>, <valid flag bitmask> } */
	{"echo", bin_echo, FLAG_N},
	{"cd", bin_cd, FLAG_L | FLAG_P},
	{"pwd", bin_pwd, 0},
	{"export", bin_export, FLAG_P},
	{"unset", bin_unset, FLAG_V | FLAG_F},  // Try adding another flag here and rerun the program to test new flag support!
	{"env", bin_env, 0},
	{"exit", bin_exit, 0},
	{NULL, NULL, 0}
```

**Example output when running minishell:**
```bash
❯ ./minishell
minishell> ls
Unknown command: ls
minishell> ls -s
Unknown command: ls -s
minishell> cd
cd builtin called
minishell> cd -p
cd builtin called
minishell> cd -n
Unknown command: cd -n
minishell> echo -n
echo builtin called
minishell> echo -k
Unknown command: echo -k
minishell>
```