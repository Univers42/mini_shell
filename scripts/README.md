## RULES
- Test exit codes, stdout, stderr separately (bats' run captures these).
- Use temporary dirs (mktemp -d) and cleanup in teardown().
- Mock external commands by placing a stub earlier in PATH in setup(), or let tests auto-stub when minishell is missing.
- Keep tests robust: compare normalized lines, avoid relying on prompts.
- Add a make target: `make test` -> runs bats locally same as CI.

## Local run (WSL Ubuntu)
```bash
sudo apt update
sudo apt install -y bats vim-common  # vim-common provides xxd
```

If your minishell is not `./minishell` in the test dir, set:
```bash
export SHELL_BIN=/home/syzygy/mini_shell/minishell
```

Run tests:
```bash
cd /home/syzygy/mini_shell/scripts/CLI
bats echo.sh
bats cd.sh
```

CI hint (Ubuntu):
```bash
- run: sudo apt-get update && sudo apt-get install -y bats vim-common
- run: cd scripts/CLI && bats echo.sh cd.sh
```