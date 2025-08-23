#!/bin/bash

prog_active() {
  local bin=$1
  local param=$2

  echo "Running: $bin -c \"$param\""
  $bin -c "$param"
}

run_both() {
  prog_active "../../minishell" "ls"
  prog_active "bash" "ls"
}

main() {
  run_both
}

main