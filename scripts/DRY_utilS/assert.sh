#!/bin/bash
set -euo pipefail

prog_active() {
  local bin=$1
  local param=$2
  local outf=$3

  if ! command -v "$bin" >/dev/null 2>&1 && [ ! -x "$bin" ]; then
    echo "ERROR: $bin not found or not executable" >&2
    return 2
  fi
  echo "Running: $bin -c \"$param\""
  "$bin" -c "$param" >"$outf" 2>&1 || true
}

run_and_compare() {
  local prog1=$1
  local prog2=$2
  local cmd="$3"

  local f1
  local f2
  f1=$(mktemp) || exit 2
  f2=$(mktemp) || (rm -f "$f1"; exit 2)
  trap 'rm -f "${f1:-}" "${f2:-}"' EXIT

  prog_active "$prog1" "$cmd" "$f1"
  prog_active "$prog2" "$cmd" "$f2"

  echo "Comparing outputs with diff -u"
  if diff -u "$f1" "$f2"; then
    echo "RESULT: IDENTICAL"
    return 0
  else
    echo "RESULT: DIFFER"
    return 1
  fi
}

main(){

  local mini_bin="../../minishell"
  local shell_bin="bash"
  local cmd="env"

  run_and_compare "$mini_bin" "$shell_bin" "$cmd"
  return $?
}

main