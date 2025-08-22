#!/usr/bin/env bats

# Source local helper assertions
. ../DRY_utilS/assert.sh

setup() {
  ms_tmpdir_setup
  ms_find_real
  if [ ! -x "$SUT" ]; then
    skip "SUT ($SUT) not found or not executable"
  fi
  # create dirs for tests
  DIR_A="$TMPDIR/dir_a"
  DIR_B="$TMPDIR/dir_b"
  RELDIR="$TMPDIR/rel_dir"
  mkdir -p "$DIR_A" "$DIR_B" "$RELDIR"
}

teardown() {
  rm -rf "$TMPDIR"
}

@test "cd to absolute path changes directory" {
  run bash -c "cd \"$TMPDIR\" && printf 'echo __PWD__\npwd\ncd %s\necho __PWD__\npwd\nexit\n' \"$DIR_A\" | \"${SUT}\""
  assert_success
  actual_norm=$(ms_extract_marked_paths "$output" "$HOME" "__PWD__")
  expected=$(printf '%s\n%s' "$TMPDIR" "$DIR_A")
  [ "$actual_norm" = "$expected" ] || { echo "Mismatch. Expected lines:"; printf '%s\n' "$expected"; echo "Actual lines:"; printf '%s\n' "$actual_norm"; return 1; }
}

@test "cd to relative path changes directory" {
  run bash -c "cd \"$TMPDIR\" && printf 'echo __PWD__\npwd\ncd rel_dir\necho __PWD__\npwd\nexit\n' | \"${SUT}\""
  assert_success
  actual_norm=$(ms_extract_marked_paths "$output" "$HOME" "__PWD__")
  expected=$(printf '%s\n%s' "$TMPDIR" "$TMPDIR/rel_dir")
  [ "$actual_norm" = "$expected" ] || { echo "Mismatch. Expected lines:"; printf '%s\n' "$expected"; echo "Actual lines:"; printf '%s\n' "$actual_norm"; return 1; }
}

@test "cd - returns to previous directory" {
  run bash -c "cd \"$TMPDIR\" && printf 'echo __PWD__\npwd\ncd %s\necho __PWD__\npwd\ncd %s\necho __PWD__\npwd\ncd -\necho __PWD__\npwd\nexit\n' \"$DIR_A\" \"$DIR_B\" | \"${SUT}\""
  assert_success
  actual_norm=$(ms_extract_marked_paths "$output" "$HOME" "__PWD__")
  # Expect strictly the four pwd snapshots: start -> DIR_A -> DIR_B -> DIR_A
  start="$TMPDIR"; a="$DIR_A"; b="$DIR_B"
  ms_assert_first_n_lines_equal "$actual_norm" "$start" "$a" "$b" "$a"
}

@test "cd with no args goes to HOME" {
  HOME_TEST="$TMPDIR/home_dir"
  mkdir -p "$HOME_TEST"
  run bash -c "cd \"$TMPDIR\" && printf 'echo __PWD__\npwd\ncd\necho __PWD__\npwd\nexit\n' | HOME=\"$HOME_TEST\" \"${SUT}\""
  assert_success
  actual_norm=$(ms_extract_marked_paths "$output" "$HOME_TEST" "__PWD__")
  expected=$(printf '%s\n%s' "$TMPDIR" "$HOME_TEST")
  [ "$actual_norm" = "$expected" ] || { echo "Mismatch. Expected lines:"; printf '%s\n' "$expected"; echo "Actual lines:"; printf '%s\n' "$actual_norm"; return 1; }
}