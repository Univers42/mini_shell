#!/usr/bin/env bash

. ../DRY_utilS/assert.sh

ms_tmpdir_setup
ms_find_real
if [ ! -x "$SUT" ]; then
  echo "SUT ($SUT) not found or not executable"
  exit 1
fi

# Test: echo prints newline by default (binary-safe check)
output=$(bash -c "printf 'echo hello\n' | \"${SUT}\" | xxd -p -c 256")
status=$?
assert_success || exit 1
assert_hex_equals "68656c6c6f0a" "$output" || exit 1

# Test: echo -n suppresses newline (binary-safe check)
output=$(bash -c "printf 'echo -n hello\n' | \"${SUT}\" | xxd -p -c 256")
status=$?
assert_success || exit 1
assert_hex_equals "68656c6c6f" "$output" || exit 1

echo "All echo tests passed."