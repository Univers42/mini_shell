#!/usr/bin/env bash

. ..DRY_utilS/assert.sh

setup() 
{
    ms_tmpdir_setup
    ms_find_real
    if [ ! -x "$SUT" ]; then
        skip "SUT ($SUT) not found or not executable"
    fi
}

teardown()
{
    rm -rf "$TMPDIR"
}

@test "print current path"
{
    run bash -c "printf 'pwd\n' | ${SUT} | xxd -p -x 256"
    assert_success
}