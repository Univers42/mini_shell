#!/bin/bash

# Export Builtin Test Script
echo "=== EXPORT BUILTIN TEST SUITE ==="

# Test 1: export without arguments (should show all exported variables)
echo -e "\n1. Test: export (no arguments)"
echo "export" | ./minishell

# Test 2: export with -p flag (same as no arguments)
echo -e "\n2. Test: export -p"
echo "export -p" | ./minishell

# Test 3: Export a new variable with value
echo -e "\n3. Test: export TESTVAR=hello"
echo -e "export TESTVAR=hello\nenv | grep TESTVAR" | ./minishell

# Test 4: Export a variable without value (empty)
echo -e "\n4. Test: export EMPTYVAR"
echo -e "export EMPTYVAR\nenv | grep EMPTYVAR" | ./minishell

# Test 5: Export multiple variables
echo -e "\n5. Test: export VAR1=value1 VAR2=value2"
echo -e "export VAR1=value1 VAR2=value2\nenv | grep VAR1\nenv | grep VAR2" | ./minishell

# Test 6: Invalid identifier (starts with number)
echo -e "\n6. Test: export 123VAR=invalid (should show error)"
echo "export 123VAR=invalid" | ./minishell

# Test 7: Invalid identifier (contains special chars)
echo -e "\n7. Test: export VAR@BAD=invalid (should show error)"
echo "export VAR@BAD=invalid" | ./minishell

# Test 8: Valid identifier with underscore
echo -e "\n8. Test: export _VALID_VAR=good"
echo -e "export _VALID_VAR=good\nenv | grep _VALID_VAR" | ./minishell

# Test 9: Export existing variable (should update)
echo -e "\n9. Test: export PATH=/new/path (update existing)"
echo -e "export OLDPATH=\$PATH\nexport PATH=/new/path\necho \$PATH" | ./minishell

# Test 10: Mixed valid and invalid exports
echo -e "\n10. Test: Mixed valid/invalid exports"
echo -e "export GOOD=value\nexport 1BAD=value\nexport ANOTHER=good" | ./minishell

echo -e "\n=== EXPORT TESTS COMPLETED ==="
