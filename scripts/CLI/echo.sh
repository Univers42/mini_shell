#!/usr/bin/env bats

# Source local helper assertions (correct path)
. ../DRY_utilS/assert.sh

setup() {
  ms_tmpdir_setup
  ms_find_real
  if [ ! -x "$SUT" ]; then
    skip "SUT ($SUT) not found or not executable"
  fi
}

teardown() {
  rm -rf "$TMPDIR"
}

@test "echo prints newline by default (binary-safe check)" {
  run bash -c "printf 'echo hello\n' | ${SUT} | xxd -p -c 256"
  assert_success
  assert_hex_equals "68656c6c6f0a" "$output"
}

@test "echo -n suppresses newline (binary-safe check)" {
  run bash -c "printf 'echo -n hello\n' | ${SUT} | xxd -p -c 256"
  assert_success
  assert_hex_equals "68656c6c6f" "$output"
}