#!/usr/bin/env bash

# need to load those utils in our functions

assert_success() {
    if [ "${status:-1}" -ne 0 ]; then
        echo "Expected success, but got failure with status ${status:-?}"
        echo "Output:"
        printf '%s\n' "${output}"
        return 1
    fi
}

assert_hex_equals()
{
    local exp="$1";
    local got="$2";
    if [ "$exp" != "$got" ]; then
        echo "Expected hex '$exp', but got '$got'"
        return 1
    fi
}

# --- shared helpers for minishell tests ---

# Ensure TMPDIR exists (idempotent)
ms_tmpdir_setup() {
  if [ -z "${TMPDIR:-}" ] || [ ! -d "$TMPDIR" ]; then
    TMPDIR="$(mktemp -d)"
    export TMPDIR
  fi
}

# Find a real minishell; sets global SUT. Do NOT create any stub.
ms_find_real() {
  # prefer existing SUT or SHELL_BIN
  if [ -n "${SUT:-}" ] && [ -x "$SUT" ]; then
    export SUT
    return 0
  fi
  if [ -n "${SHELL_BIN:-}" ] && [ -x "$SHELL_BIN" ]; then
    SUT="$SHELL_BIN"
    export SUT
    return 0
  fi
  if [ -x "./minishell" ]; then
    SUT="./minishell"
    export SUT
    return 0
  fi

  # robustly search in the user's home directory; use $HOME (quoted) not ~
  local found
  if [ -n "${HOME:-}" ]; then
    found=$(find "$HOME" -L -type f -name minishell -executable 2>/dev/null | head -n1 || true)
  else
    found=""
  fi

  if [ -n "$found" ]; then
    SUT="$found"
    export SUT
  else
    unset SUT
  fi

  # Always return success so bats setup can handle skip/abort logic itself
  return 0
}

# Normalize a single line: map ~ and ~/X to home path
ms_normalize_line() {
  local line="$1"; local homepath="$2"
  if [ "$line" = "~" ]; then
    line="$homepath"
  elif [[ "$line" == "~/"* ]]; then
    line="${homepath}/${line#~/}"
  fi
  # canonicalize: strip trailing slashes except root
  if [ "$line" != "/" ]; then
    while [[ "$line" == */ && "$line" != "/" ]]; do line="${line%/}"; done
  fi
  printf '%s\n' "$line"
}

# Normalize output: keep only path-like lines, strip empties, map ~ to home
ms_normalize_output_lines() {
  local out="$1"; local homepath="$2"
  local norm=""
  IFS=$'\n' read -r -d '' -a lines <<<"$out"$'\0'
  for i in "${!lines[@]}"; do
    local line="${lines[$i]}"
    # trim whitespace
    line="${line#"${line%%[![:space:]]*}"}"
    line="${line%"${line##*[![:space:]]}"}"
    [ -z "$line" ] && continue
    # keep path-like lines: start with / or ~, or contain TMPDIR marker
    if [[ ! "$line" =~ ^(/|~) ]] && [[ -n "$TMPDIR" ]] && [[ "$line" != *"$TMPDIR"* ]]; then
      continue
    fi
    local l; l="$(ms_normalize_line "$line" "$homepath")"
    if [ -n "$norm" ]; then norm+=$'\n'; fi
    norm+="$l"
  done
  printf '%s' "$norm"
}

# Extract only path outputs that follow a given marker line (e.g., '__PWD__'),
# normalize them against the provided HOME path, and return newline-joined list.
ms_extract_marked_paths() {
  local out="$1"; local homepath="$2"; local marker="$3"
  local collected=()
  IFS=$'\n' read -r -d '' -a lines <<<"$out"$'\0'
  local want_next=0
  for ((i=0;i<${#lines[@]};i++)); do
    # trim surrounding whitespace
    local line="${lines[$i]}"
    line="${line#"${line%%[![:space:]]*}"}"
    line="${line%"${line##*[![:space:]]}"}"
    if [ "$want_next" -eq 1 ]; then
      # skip empties and non-path-like until we see a path
      if [ -z "$line" ]; then continue; fi
      if [[ "$line" =~ ^(/|~) ]] || { [[ -n "$TMPDIR" ]] && [[ "$line" == *"$TMPDIR"* ]]; }; then
        collected+=( "$(ms_normalize_line "$line" "$homepath")" )
        want_next=0
        continue
      else
        # not a path-like line; keep waiting
        continue
      fi
    fi
    # set marker
    if [ "$line" = "$marker" ]; then
      want_next=1
    fi
  done
  printf '%s' "${collected[*]}" | tr ' ' '\n'
}

# Assert expected lines appear in order (subsequence)
ms_assert_sequence_in_order() {
  local actual="$1"; shift
  local expected=("$@")
  local idx=0
  IFS=$'\n' read -r -d '' -a alines <<<"$actual"$'\0'
  for ((i=0;i<${#alines[@]};i++)); do
    if [ "${alines[i]}" = "${expected[idx]}" ]; then
      ((idx++))
      if [ $idx -ge ${#expected[@]} ]; then return 0; fi
    fi
  done
  echo "Expected sequence not found in order."
  echo "Expected:"; for e in "${expected[@]}"; do echo "  $e"; done
  echo "Actual:"; for a in "${alines[@]}"; do echo "  $a"; done
  return 1
}

# Assert the first N lines equal expected (robust against extra lines later)
ms_assert_first_n_lines_equal() {
  local actual="$1"; shift
  local expected=("$@")
  IFS=$'\n' read -r -d '' -a alines <<<"$actual"$'\0'
  local n=${#expected[@]}
  if [ "${#alines[@]}" -lt "$n" ]; then
    echo "Actual has fewer lines (${#alines[@]}) than expected ($n)"; return 1
  fi
  for ((i=0;i<n;i++)); do
    if [ "${alines[i]}" != "${expected[i]}" ]; then
      echo "Line $((i+1)) mismatch: expected '${expected[i]}', got '${alines[i]}'"
      return 1
    fi
  done
  return 0
}