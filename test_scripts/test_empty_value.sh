#!/bin/bash

echo "=== Testing empty string values ==="

# Create a test file for minishell input
cat > minishell_empty_test.txt << 'EOF'
export hi=""
export | grep hi
env | grep hi
exit
EOF

echo "Testing minishell with empty string value:"
./minishell < minishell_empty_test.txt

rm -f minishell_empty_test.txt

echo
echo "=== Testing bash empty string for comparison ==="
bash -c "export hi=''; export | grep hi; env | grep hi" 