#!/usr/bin/env bash

set -e

TEST_DIR="$(dirname "$0")/CLI"
FAILED=0

for script in "$TEST_DIR"/*.sh; do
    echo "Running $(basename "$script")..."
    if bash "$script"; then
        echo "✅ $(basename "$script") passed."
    else
        echo "❌ $(basename "$script") failed."
        FAILED=1
    fi
    echo
done

if [ "$FAILED" -eq 0 ]; then
    echo "All tests passed!"
    exit 0
else
    echo "Some tests failed."
    exit 1
fi
