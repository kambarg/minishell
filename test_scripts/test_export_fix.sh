#!/bin/bash

echo "Testing export functionality fix..."
echo

echo "Test 1: export mhi (without value)"
echo "export mhi" | ./minishell

echo "Test 2: Check if mhi appears in export output"
echo "export | grep mhi" | ./minishell

echo "Test 3: Check if mhi appears in env output (should not)"
echo "env | grep mhi" | ./minishell

echo "Test 4: export hi=\"\" (with empty value)"
echo "export hi=\"\"" | ./minishell

echo "Test 5: Check if hi appears in export output"
echo "export | grep hi" | ./minishell

echo "Test 6: Check if hi appears in env output (should show empty)"
echo "env | grep hi" | ./minishell

echo "Test 7: unset mhi"
echo "unset mhi" | ./minishell

echo "Test 8: Check if mhi still appears in export (should not)"
echo "export | grep mhi" | ./minishell 